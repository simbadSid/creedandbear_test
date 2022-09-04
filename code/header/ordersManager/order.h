//
// Created by Riyane on 01/09/2022.
//

#ifndef TEST_ORDER_H
#define TEST_ORDER_H

class Order
{
public:
    int id;
    unsigned int number;

    Order();
    Order(int id, unsigned int number);
//    Order(Order const &o);
bool operator == (Order o)
{
    return ((this->id = o.id) && (this->number = o.number));
}

};


#endif //TEST_ORDER_H
