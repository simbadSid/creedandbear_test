//
// Created by Riyane on 31/08/2022.
//

#include "ordersManager_4_sharedList_BufferedList_BufferedDb.h"


//TODO handle the SIGINT







void OrdersManager_4_sharedList_BufferedList_BufferedDb::custom_fake_save_on_db(int order_id, unsigned int order_number)
{
    this->buffer[this->currentBufferSize] = std::pair<int, unsigned int>(order_id, order_number);
    ++ this->currentBufferSize;

    if (this->currentBufferSize == BUFFER_SIZE_LIST)
    {
        std::lock_guard<std::mutex> lockGuard(this->mtx); // here a lock_guard is enough
        for(const auto& order: this->buffer)
        {
            this->sharedList->push_front(order);
        }
        this->condSharedAccess.notify_all();

        this->currentBufferSize = 0;
    }
}

void OrdersManager_4_sharedList_BufferedList_BufferedDb::orderConsumer()
{
    std::pair<int, unsigned int> orderArray[BUFFER_SIZE_LIST];

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
                this->log("Got request to shut down thread %d\n", std::this_thread::get_id());
                this->condSharedAccess.notify_all();
                break;
            }
            if (this->sharedList->empty())
                continue;

            // Retrieve all the orders to process (buffer size)
            for (int i=0; i<BUFFER_SIZE_LIST && !this->sharedList->empty(); i++)
            {
                orderArray[i] = this->sharedList->back();
                this->sharedList->pop_back();
            }

            // Release the lock
            this->condSharedAccess.notify_all();
        }
        this->fake_save_on_db(orderArray, BUFFER_SIZE_LIST);
    }
}

void OrdersManager_4_sharedList_BufferedList_BufferedDb::startOrderManager()
{
    // Init the buffer and shared list
    this->currentBufferSize = 0;
    this->sharedList = new std::list<std::pair<int, unsigned int> >(0);

    // Init the consumer threads
    this->run = true;
    for(unsigned i = 0; i < NB_THREAD; ++i)
    {
        threadVector[i] = std::thread(&OrdersManager_4_sharedList_BufferedList_BufferedDb::orderConsumer, this);
        this->log("Init thread %d\n", threadVector[i].get_id());
    }
    this->process_orders();
}

void OrdersManager_4_sharedList_BufferedList_BufferedDb::waitAndCleanOrderManager()
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
        auto threadId = threadVector[i].get_id();
        this->condSharedAccess.notify_all();
        this->threadVector[i].join();
        this->log("End thread %d\n", threadId);
    }
    delete this->sharedList;

    // Store the remaining in the buffer
    this->fake_save_on_db(this->buffer, this->currentBufferSize);
    this->currentBufferSize = 0;
}

