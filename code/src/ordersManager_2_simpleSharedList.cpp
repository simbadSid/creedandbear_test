//
// Created by Riyane on 31/08/2022.
//

#include "OrdersManager_2_simpleSharedList.h"


//TODO handle the SIGINT

static bool run;


OrdersManager_2_simpleSharedList::~OrdersManager_2_simpleSharedList()
{
    this->stopAndCleanOrderManager();
}

void OrdersManager_2_simpleSharedList::custom_fake_save_on_db(int order_id, unsigned int order_number)
{
//TODO
    std::lock_guard<std::mutex> lockGuard(this->mtx); // here a lock_guard is enough
    std::pair<int, unsigned int> order(order_id, order_number);
    this->sharedList.push_front(order);
//TODO no manual unlocking
    cond.notify_all();
//TODO check if notify_one
}

void OrdersManager_2_simpleSharedList::orderConsumer()
{
    while (run)
    {
        std::unique_lock<std::mutex> lockGuard(this->mtx);
        cond.wait(lockGuard, [this] { return run || !this->sharedList.empty(); });
        if (!run)
            break;
        std::pair<int, unsigned int> order = this->sharedList.back();
        this->sharedList.pop_back();
        this->fake_save_on_db(order.first, order.second);
//TODO check if release the lock
    }
}

void OrdersManager_2_simpleSharedList::startOrderManager()
{
    // Init the consumer threads
    run = true;
    for(unsigned i = 0; i < NB_THREAD; ++i)
    {
//        threadVector.emplace_back(&OrdersManager_2_simpleSharedList::orderConsumer, this);
        threadVector[i] = std::thread(&OrdersManager_2_simpleSharedList::orderConsumer, this);
        this->log("Init thread %d\n", threadVector[i].get_id());
    }
}

void OrdersManager_2_simpleSharedList::stopAndCleanOrderManager()
{
    std::lock_guard<std::mutex> lockGuard(this->mtx);
    run = false;
    this->cond.notify_all();
    for (unsigned i=0; i<NB_THREAD; ++i)
    {
        this->threadVector[i].join();
        this->log("End thread %d\n", threadVector[i].get_id());
    }
}

