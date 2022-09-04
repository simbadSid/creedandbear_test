//
// Created by Riyane on 03/09/2022.
//


#include "order.h"

Order::Order(int id, unsigned int number)
{
    this->id = id;
    this->number = number;
}

bool Order::operator==(const Order & o) const
{
    return ((this->id == o.id) && (this->number == o.number));
}
