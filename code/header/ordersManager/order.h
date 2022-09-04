//
// Created by Riyane on 01/09/2022.
//

#ifndef TEST_ORDER_H
#define TEST_ORDER_H

#include <iostream>

class Order
{
public:
    int id;
    unsigned int number;

    Order() = default;
    Order(int id, unsigned int number);

    bool operator==(const Order & o) const;

    struct HashFunction
    {
        size_t operator() (const  Order & o) const
        {
            size_t hash_id     = std::hash<int>         ()(o.id);
            size_t hash_number = std::hash<unsigned int>()(o.number) << 1;

            return hash_id ^ hash_number;
        }
    };

};


#endif //TEST_ORDER_H
