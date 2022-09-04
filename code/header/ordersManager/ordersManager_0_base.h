//
// Created by Riyane on 31/08/2022.
//

#ifndef CREEDANDBEAR_TEST_ORDERSMANAGER_0_BASE_H
#define CREEDANDBEAR_TEST_ORDERSMANAGER_0_BASE_H

#include "ordersManager.h"

class OrdersManager_0_base : public OrdersManager
{
protected:
    void custom_fake_save_on_db(Order order);

};
#endif //CREEDANDBEAR_TEST_ORDERSMANAGER_0_BASE_H
