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
#include <list>

#include "order.h"




#define NB_THREAD           5
#define BUFFER_SIZE_DB      5
#define BUFFER_SIZE_LIST    50
#define DB_ACCESS_TIME      10 // Maximum time (n Milliseconds) to access a DB



class OrdersManager
{
private:
    std::vector<Order> orders;
    unsigned int orders_processed;
    std::chrono::system_clock::time_point last_printed_log;
    unsigned int quantity;


public:
    OrdersManager();
    virtual ~OrdersManager() = default;

    void generate_fake_orders(unsigned int quantity);
    void fake_save_on_db(Order order);
    void fake_save_on_db(const Order orderList[], unsigned int size);

    /**
     * @def Main function: run the simulation by processing all the created orders
     * @param om OrdersManager on which to run the simulation
     * @return the execution time of the simulation (in milliseconds)
     */
    static long runOrdersManager(OrdersManager *om, unsigned int quantity);


protected:
    virtual void custom_fake_save_on_db(Order order) = 0;
    void process_orders();
    virtual void startOrderManager();
    virtual void waitAndCleanOrderManager();


//#define TEST
#ifdef TEST
private:
    std::mutex mtx; // Mutex used to store orders for unit tests
    std::vector<Order> ordersinDB;
    unsigned int ordersinDBIndex;

public:
    bool isCorrectOrdersProcessing();
#endif

};

#ifdef LOG
void log(const char *format, ...);
#else
#define log(...) ((void)0)
#endif



#endif //CREEDANDBEAR_TEST_ORDERSMANAGER_H
