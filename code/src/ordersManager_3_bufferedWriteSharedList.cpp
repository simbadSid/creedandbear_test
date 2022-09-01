//
// Created by Riyane on 31/08/2022.
//

#include "OrdersManager_3_bufferedWriteSharedList.h"


//TODO handle the SIGINT

static bool run;


OrdersManager_3_bufferedWriteSharedList::~OrdersManager_3_bufferedWriteSharedList()
{
    this->stopAndCleanOrderManager();
}

void OrdersManager_3_bufferedWriteSharedList::custom_fake_save_on_db(int order_id, unsigned int order_number)
{
    this->bufferToSave[this->currentBufferSize] = std::pair<int, unsigned int>(order_id, order_number);
    ++ this->currentBufferSize;

    if (this->currentBufferSize == BUFFER_SIZE)
    {
        std::lock_guard<std::mutex> lockGuard(this->mtx); // here a lock_guard is enough
        for(const auto& order: this->bufferToSave)
        {
            this->sharedList.push_front(order);
        }
        cond.notify_all();

        this->currentBufferSize = 0;
    }
    cond.notify_all();
}

void OrdersManager_3_bufferedWriteSharedList::orderConsumer()
{
    while (run)
    {
        std::unique_lock<std::mutex> lockGuard(this->mtx);
        cond.wait(lockGuard, [this] { return !run || !this->sharedList.empty(); });
        if (!run)
            break;
        std::pair<int, unsigned int> order = this->sharedList.back();
        this->sharedList.pop_back();
        this->fake_save_on_db(order.first, order.second);
//TODO check if release the lock
    }
}

void OrdersManager_3_bufferedWriteSharedList::startOrderManager()
{
    // Init the buffer
    this->bufferToSave = std::vector<std::pair<int, unsigned int> >(BUFFER_SIZE);
    this->currentBufferSize = 0;

    // Init the consumer threads
    this->threadVector.reserve(NB_THREAD);
    run = true;
    for(unsigned i = 0; i < NB_THREAD; ++i)
    {
        threadVector.emplace_back(&OrdersManager_3_bufferedWriteSharedList::orderConsumer, this);
        this->log("Init thread %d\n", threadVector[i].get_id());
    }
}

void OrdersManager_3_bufferedWriteSharedList::stopAndCleanOrderManager()
{
    run = false;
    this->cond.notify_all();
    std::lock_guard<std::mutex> lockGuard(this->mtx);
    for (unsigned i=0; i<NB_THREAD; ++i)
    {
        this->cond.notify_all();
        this->threadVector[i].join();
        this->log("End thread %d\n", threadVector[i].get_id());
    }

    for (unsigned i=0; i<this->currentBufferSize; ++i)
    {
        auto order = this->bufferToSave[this->currentBufferSize];
        this->fake_save_on_db(order.first, order.second);
    }


    //TODO empty the buffer
}

