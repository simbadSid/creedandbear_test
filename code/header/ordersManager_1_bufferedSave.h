//
// Created by Riyane on 31/08/2022.
//

#ifndef CREEDANDBEAR_TEST_ORDERSMANAGER_1_BUFFEREDSAVE_H
#define CREEDANDBEAR_TEST_ORDERSMANAGER_1_BUFFEREDSAVE_H

#include "ordersManager.h"

class OrdersManager_1_bufferedSave : public OrdersManager
{
private:
    #define BUFFER_SIZE 5
    std::vector<std::pair<int, unsigned int> > bufferToSave;
    unsigned int currentBufferSize;

public:
    ~OrdersManager_1_bufferedSave();

protected:
    void custom_fake_save_on_db(int order_id, unsigned int order_number);

    void startOrderManager();
    void stopAndCleanOrderManager();

};
#endif //CREEDANDBEAR_TEST_ORDERSMANAGER_1_BUFFEREDSAVE_H
