//
// Created by Riyane on 31/08/2022.
//

#include "OrdersManager_1_bufferedSave.h"


OrdersManager_1_bufferedSave::~OrdersManager_1_bufferedSave()
{
    this->stopAndCleanOrderManager();
}

void OrdersManager_1_bufferedSave::custom_fake_save_on_db(int order_id, unsigned int order_number)
{
    this->bufferToSave[this->currentBufferSize] = std::pair<int, unsigned int>(order_id, order_number);
    ++ this->currentBufferSize;

    if (this->currentBufferSize == BUFFER_SIZE)
    {
        for(const auto& order: this->bufferToSave)
        {
            this->fake_save_on_db(order.first, order.second);
        }
        this->currentBufferSize = 0;
    }
}

void OrdersManager_1_bufferedSave::startOrderManager()
{
    this->bufferToSave = std::vector<std::pair<int, unsigned int> >(BUFFER_SIZE);
    this->currentBufferSize = 0;
}

void OrdersManager_1_bufferedSave::stopAndCleanOrderManager()
{
    for (unsigned i=0; i<this->currentBufferSize; ++i)
    {
        auto order = this->bufferToSave[this->currentBufferSize];
        this->fake_save_on_db(order.first, order.second);
    }
}

