//
// Created by Riyane on 31/08/2022.
//

#include "ordersManager_5_perThreadList_BufferedList_BufferedDb.h"


//TODO handle the SIGINT







void OrdersManager_5_perThreadList_BufferedList_BufferedDb::custom_fake_save_on_db(Order order)
{
    this->buffer[this->currentBufferSize] = order;
    ++ this->currentBufferSize;

    if (this->currentBufferSize == BUFFER_SIZE_DB)
    {
        ThreadAttribute *threadAttribute = this->threadAttributeList + this->currentThreadIndex;
        std::lock_guard<std::mutex> lockGuard(threadAttribute->mtx); // here a lock_guard is enough
        for (unsigned int i=0; i<BUFFER_SIZE_DB; ++i)
            threadAttribute->sharedList->push_front(this->buffer[i]);
        threadAttribute->condSharedAccess.notify_one();

        this->currentBufferSize = 0;
        this->currentThreadIndex = (this->currentThreadIndex + 1) % NB_THREAD;
    }
}

void OrdersManager_5_perThreadList_BufferedList_BufferedDb::orderConsumer(unsigned int threadIndex)
{
    ThreadAttribute *threadAttribute = this->threadAttributeList + threadIndex;
    Order *orderArray;
    unsigned int nbElem;

    while (threadAttribute->run)
    {
        // Scope of the definition of the lock (released after the scope)
        {
            // Wait for an order to be present
            std::unique_lock<std::mutex> lockGuard(threadAttribute->mtx);
            if (threadAttribute->sharedList->empty())
            {
                threadAttribute->condSharedAccess.notify_one();
                threadAttribute->condSharedAccess.wait(lockGuard);
            }
            if (!threadAttribute->run)
            {
                log("Got request to shut down thread %d (%d)\n", std::this_thread::get_id(), threadIndex);
                threadAttribute->condSharedAccess.notify_all();
                break;
            }
            if (threadAttribute->sharedList->empty())
                continue;

            // Retrieve all the orders to process (N * buffer size)
            nbElem = threadAttribute->sharedList->size();
            orderArray = new Order[nbElem];
            for (unsigned int i=0; i<nbElem; ++i)
            {
                orderArray[i] = threadAttribute->sharedList->back();
                threadAttribute->sharedList->pop_back();
            }

            // Release the lock
            threadAttribute->condSharedAccess.notify_all();
        }
        this->fake_save_on_db(orderArray, nbElem);
        delete orderArray;
    }
}

void OrdersManager_5_perThreadList_BufferedList_BufferedDb::startOrderManager()
{
    this->currentBufferSize = 0;
    this->currentThreadIndex = 0;

    for (unsigned int i=0; i<NB_THREAD; i++)
    {
        // Init the buffer and shared list of the thread
        this->threadAttributeList[i].sharedList = new std::list<Order>(0);

        // Init the consumer thread
        this->threadAttributeList[i].run = true;
        this->threadAttributeList[i].thread = std::thread(&OrdersManager_5_perThreadList_BufferedList_BufferedDb::orderConsumer, this, i);
        log("Init thread %d (%d)\n", this->threadAttributeList[i].thread.get_id(), i);
    }

    this->process_orders();
}

void OrdersManager_5_perThreadList_BufferedList_BufferedDb::waitAndCleanOrderManager()
{
    // For each trade
    ThreadAttribute *threadAttribute = this->threadAttributeList;
    for (unsigned i=0; i<NB_THREAD; ++i)
    {
        // Wait for the end of the thread
        while (true)
        {
            std::unique_lock<std::mutex> lockGuard(threadAttribute->mtx);
            if (threadAttribute->sharedList->empty())
                break;
            threadAttribute->condSharedAccess.notify_one();
            threadAttribute->condSharedAccess.wait(lockGuard, [threadAttribute] {return threadAttribute->sharedList->empty(); });
        }

        threadAttribute->run = false;

        // Join the thread
        threadAttribute->condSharedAccess.notify_all();
        threadAttribute->thread.join();
        log("End thread %d (%d)\n", threadAttribute->thread.get_id(), i);
        delete threadAttribute->sharedList;

        ++threadAttribute;
    }

    // Store the remaining from the producer buffer
    this->fake_save_on_db(this->buffer, this->currentBufferSize);
    this->currentBufferSize = 0;
}

