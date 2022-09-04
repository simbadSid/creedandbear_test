//
// Created by Riyane on 31/08/2022.
//

#include "ordersManager_1_bufferedDb.h"



void OrdersManager_1_bufferedDb::custom_fake_save_on_db(Order order)
{
    this->bufferDB[this->currentBufferSize] = order;
    ++ this->currentBufferSize;

    if (this->currentBufferSize == BUFFER_SIZE_DB)
    {
        this->fake_save_on_db(this->bufferDB, BUFFER_SIZE_DB);
        this->currentBufferSize = 0;
    }
}

void OrdersManager_1_bufferedDb::startOrderManager()
{
    this->currentBufferSize = 0;
    this->process_orders();
}

void OrdersManager_1_bufferedDb::waitAndCleanOrderManager()
{
    // Store the remaining in the buffer
    this->fake_save_on_db(this->bufferDB, this->currentBufferSize);
    this->currentBufferSize = 0;
}

