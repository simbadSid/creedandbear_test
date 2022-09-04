//
// Created by Riyane on 31/08/2022.
//

#include "ordersManager_3_sharedList_BufferedList.h"


//TODO handle the SIGINT







void OrdersManager_3_sharedList_BufferedList::custom_fake_save_on_db(Order order)
{
    this->bufferSharedList[this->currentBufferSize] = order;
    ++ this->currentBufferSize;

    if (this->currentBufferSize == BUFFER_SIZE_LIST)
    {
        std::lock_guard<std::mutex> lockGuard(this->mtx); // here a lock_guard is enough
        for(const auto& order: this->bufferSharedList)
        {
            this->sharedList->push_front(order);
        }
        this->condSharedAccess.notify_all();

        this->currentBufferSize = 0;
    }
}

void OrdersManager_3_sharedList_BufferedList::orderConsumer()
{
    Order order;

    while (this->run)
    {
        // Scope of the definition of the lock (released after the scope)
        {
            // Wait for an order to be present
            std::unique_lock<std::mutex> lockGuard(this->mtx);
            this->condSharedAccess.notify_all();
            this->condSharedAccess.wait(lockGuard);
            if (!this->run)
            {
                log("Got request to shut down thread %d\n", std::this_thread::get_id());
                this->condSharedAccess.notify_all();
                break;
            }
            if (this->sharedList->empty())
                continue;

            // Retrieve the order to process
            order = this->sharedList->back();
            this->sharedList->pop_back();

            // Release the lock
            this->condSharedAccess.notify_all();
        }
        this->fake_save_on_db(order);
    }
}

void OrdersManager_3_sharedList_BufferedList::startOrderManager()
{
    // Init the buffer and shared list
    this->currentBufferSize = 0;
    this->sharedList = new std::list<Order>(0);

    // Init the consumer threads
    this->run = true;
    for(unsigned i = 0; i < NB_THREAD; ++i)
    {
        threadVector[i] = std::thread(&OrdersManager_3_sharedList_BufferedList::orderConsumer, this);
        log("Init thread %d\n", threadVector[i].get_id());
    }
    this->process_orders();
}

void OrdersManager_3_sharedList_BufferedList::waitAndCleanOrderManager()
{
    // Wait for the end of each consumer thread
    while(true)
    {
        std::unique_lock<std::mutex> lockGuard(this->mtx);
        if (this->sharedList->empty())
            break;
        this->condSharedAccess.notify_all();
        this->condSharedAccess.wait(lockGuard, [this] { return this->sharedList->empty(); });
    }

    // Clean all the threads
    this->run = false;
    for (unsigned i=0; i<NB_THREAD; ++i)
    {
        this->condSharedAccess.notify_all();
        this->threadVector[i].join();
        log("End thread %d\n", threadVector[i].get_id());
    }
    delete this->sharedList;

    // Store the remaining in the buffer
    this->fake_save_on_db(this->bufferSharedList, this->currentBufferSize);
    this->currentBufferSize = 0;
}

