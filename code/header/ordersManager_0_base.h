//
// Created by Riyane on 31/08/2022.
//

#ifndef CREEDANDBEAR_TEST_ORDERSMANAGER_0_BASE_H
#define CREEDANDBEAR_TEST_ORDERSMANAGER_0_BASE_H

#include "ordersManager.h"

class OrdersManager_0_base : public OrdersManager
{
public:
    OrdersManager_0_base(unsigned int quantity) : OrdersManager(quantity){};
    ~OrdersManager_0_base();

protected:
    void custom_fake_save_on_db(int order_id, unsigned int order_number);

private:
    void stopAndCleanOrderManager();

};
#endif //CREEDANDBEAR_TEST_ORDERSMANAGER_0_BASE_H
