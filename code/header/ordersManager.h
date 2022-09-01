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
    OrdersManager();
    virtual ~OrdersManager() = default;

    void generate_fake_orders(unsigned int quantity);
    void log(const char *format, ...);
    void fake_save_on_db(int order_id, unsigned int order_number);
    void process_orders();

    /**
     * @def Main function: run the simulation by processing all the created orders
     * @param om OrdersManager on which to run the simulation
     * @return the execution time of the simulation (in milliseconds)
     */
    static long runOrdersManager(OrdersManager *om, unsigned int quantity);


protected:
    virtual void custom_fake_save_on_db(int order_id, unsigned int order_number) = 0;
    virtual void startOrderManager() = 0;
    virtual void stopAndCleanOrderManager() = 0;
};



#endif //CREEDANDBEAR_TEST_ORDERSMANAGER_H
