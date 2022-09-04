//
// Created by Riyane on 03/09/2022.
//

#include "order.h"

Order::Order()
{

}

Order::Order(int id, unsigned int number)
{
    this->id = id;
    this->number = number;
}
/*
Order::Order(Order const &o)
{
    this->id = o.id;
    this->number = o.number;
}
*/