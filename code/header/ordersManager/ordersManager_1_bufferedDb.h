//
// Created by Riyane on 31/08/2022.
//

#ifndef CREEDANDBEAR_TEST_ORDERSMANAGER_1_BUFFEREDDB_H
#define CREEDANDBEAR_TEST_ORDERSMANAGER_1_BUFFEREDDB_H

#include "ordersManager.h"

class OrdersManager_1_bufferedDb : public OrdersManager
{
private:
    Order bufferDB[BUFFER_SIZE_DB];
    unsigned int currentBufferSize;

protected:
    void custom_fake_save_on_db(Order order);

    void startOrderManager();
    void waitAndCleanOrderManager();

};
#endif //CREEDANDBEAR_TEST_ORDERSMANAGER_1_BUFFEREDDB_H
