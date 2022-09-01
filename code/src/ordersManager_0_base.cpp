//
// Created by Riyane on 31/08/2022.
//

#include "ordersManager_0_base.h"


OrdersManager_0_base::~OrdersManager_0_base()
{
    this->stopAndCleanOrderManager();
}

void OrdersManager_0_base::custom_fake_save_on_db(int order_id, unsigned int order_number)
{
    this->fake_save_on_db(order_id, order_number);
}

void OrdersManager_0_base::startOrderManager()
{

}

void OrdersManager_0_base::stopAndCleanOrderManager()
{

}

