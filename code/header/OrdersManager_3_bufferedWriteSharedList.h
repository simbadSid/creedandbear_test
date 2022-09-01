//
// Created by Riyane on 31/08/2022.
//

#ifndef CREEDANDBEAR_TEST_ORDERSMANAGER_2_BUFFERED_WRITE_SHARED_LIST_H
#define CREEDANDBEAR_TEST_ORDERSMANAGER_2_BUFFERED_WRITE_SHARED_LIST_H

#include <thread>
#include <list>
#include "ordersManager.h"

class OrdersManager_3_bufferedWriteSharedList : public OrdersManager
{
private:
    #define BUFFER_SIZE 5
    std::vector<std::pair<int, unsigned int> > bufferToSave;
    unsigned int currentBufferSize;

    std::mutex mtx;
    std::condition_variable cond;
    #define NB_THREAD 5
    std::vector<std::thread> threadVector;
    std::list<std::pair<int, unsigned int> > sharedList;

public:
        ~OrdersManager_3_bufferedWriteSharedList();

protected:
    /**
     * @def This method is an asynchronous write in DB: It simply waked a consumer thread that will execute the DB request.
     * This method will not wait for the request inorder to return.
     * This method does not write the new order in the shared list at each call. Instead, it store each new call within a temporary buffer until the buffer is full
     * @param order_id
     * @param order_number
     */
    void custom_fake_save_on_db(int order_id, unsigned int order_number);
    void orderConsumer();
    void startOrderManager();
    void stopAndCleanOrderManager();

};
#endif //CREEDANDBEAR_TEST_ORDERSMANAGER_2_BUFFERED_WRITE_SHARED_LIST_H
