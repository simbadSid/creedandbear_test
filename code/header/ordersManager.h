//
// Created by Riyane on 31/08/2022.
//

#ifndef CREEDANDBEAR_TEST_ORDERSMANAGER_H
#define CREEDANDBEAR_TEST_ORDERSMANAGER_H

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <random>






class OrdersManager {
private:
    std::vector<std::pair<int, unsigned int> > orders;
    unsigned int orders_processed;
    std::chrono::system_clock::time_point last_printed_log;
    unsigned int quantity;


public:
    OrdersManager(unsigned int quantity);

    void generate_fake_orders();
    void log(const char *format, ...);
    void fake_save_on_db(int order_id, unsigned int order_number);
    void process_orders();

    static void runOrdersManager(OrdersManager *om);


protected:
    virtual void custom_fake_save_on_db(int order_id, unsigned int order_number) = 0;
};



#endif //CREEDANDBEAR_TEST_ORDERSMANAGER_H
