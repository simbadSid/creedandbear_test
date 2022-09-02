//
// Created by Riyane on 31/08/2022.
//

#ifndef CREEDANDBEAR_TEST_ORDERSMANAGER_1_BUFFEREDDB_H
#define CREEDANDBEAR_TEST_ORDERSMANAGER_1_BUFFEREDDB_H

#include "ordersManager.h"

class OrdersManager_1_bufferedDb : public OrdersManager
{
private:
    std::pair<int, unsigned int> bufferDB[BUFFER_SIZE_DB];
    unsigned int currentBufferSize;

protected:
    void custom_fake_save_on_db(int order_id, unsigned int order_number);

    void startOrderManager();
    void waitAndCleanOrderManager();

};
#endif //CREEDANDBEAR_TEST_ORDERSMANAGER_1_BUFFEREDDB_H
