//
// Created by Riyane on 31/08/2022.
//

#ifndef CREEDANDBEAR_TEST_ORDERSMANAGER_2_SHARED_LIST_H
#define CREEDANDBEAR_TEST_ORDERSMANAGER_2_SHARED_LIST_H

#include <thread>
#include <list>
#include "ordersManager.h"

class OrdersManager_2_sharedList : public OrdersManager
{
private:
    std::mutex mtx;
    std::condition_variable condSharedAccess;
    std::thread threadVector[NB_THREAD];
    std::list<std::pair<int, unsigned int> > *sharedList;
    // The atomic is not necessary in this context
    // It is used to close the warning about data set but not used
    std::atomic<bool> run;

protected:
    /**
     * @def This method is an asynchronous write in DB: It simply wakes a consumer thread that will execute the DB request.
     * This method returns before the consumer thread has effectively stored the order in the DB.
     * @param order_id
     * @param order_number
     */
    void custom_fake_save_on_db(int order_id, unsigned int order_number);
    void orderConsumer();
    void startOrderManager();
    void waitAndCleanOrderManager();

};
#endif //CREEDANDBEAR_TEST_ORDERSMANAGER_2_SHARED_LIST_H
