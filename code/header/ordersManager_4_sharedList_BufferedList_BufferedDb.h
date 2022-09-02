//
// Created by Riyane on 31/08/2022.
//

#ifndef CREEDANDBEAR_TEST_ORDERSMANAGER_4_SHARED_LIST_BUFFERED_LIST_BUFFERED_DB_H
#define CREEDANDBEAR_TEST_ORDERSMANAGER_4_SHARED_LIST_BUFFERED_LIST_BUFFERED_DB_H

#include <thread>
#include <list>
#include "ordersManager.h"

class OrdersManager_4_sharedList_BufferedList_BufferedDb : public OrdersManager
{
private:
    std::pair<int, unsigned int> buffer[BUFFER_SIZE_LIST];
    unsigned int currentBufferSize;

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
     * This method does not write the new order in the shared list at each call. Instead, it store each new call within a temporary buffer until the buffer is full.
     * This allows to reduce the need to synchronization between producer and consumers threads.
     * @param order_id
     * @param order_number
     */
    void custom_fake_save_on_db(int order_id, unsigned int order_number);
    void orderConsumer();
    void startOrderManager();
    void waitAndCleanOrderManager();

};
#endif //CREEDANDBEAR_TEST_ORDERSMANAGER_4_SHARED_LIST_BUFFERED_LIST_BUFFERED_DB_H
