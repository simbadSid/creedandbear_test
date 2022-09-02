//
// Created by Riyane on 31/08/2022.
//

#include "ordersManager.h"


std::random_device rd; // obtain a random number from hardware
std::mt19937 gen(rd()); // seed the generator
std::uniform_int_distribution<> distr_accessTime_single(0, DB_ACCESS_TIME);
std::uniform_int_distribution<> distr_accessTime_multiple(0, 2*DB_ACCESS_TIME);


OrdersManager::OrdersManager()
{
    // Use a seed for the random generator
    std::srand(42);
}

void OrdersManager::generate_fake_orders(unsigned int quantity)
{
    this->log("\n\nGenerating fake orders: %u \n", quantity);

    this->orders = std::vector<std::pair<int, unsigned int> >(quantity);
    this->orders_processed = 0;
    this->last_printed_log = std::chrono::system_clock::now();
    this->quantity = quantity;

    for (unsigned int i=0; i<this->quantity; ++i)
    {
        std::pair<int, int> p(std::rand(), i);
        this->orders[i] = p;
    }

    log("%u generated...\n", this->quantity);
}

void OrdersManager::log(const char *format, ...)
{
//TODO ad date time in logger

#ifdef LOG
    va_list ap;
    va_start (ap, format);
    vfprintf (stdout, format, ap);
    va_end (ap);
    fflush (stdout);
#endif
}

void OrdersManager::fake_save_on_db(int order_id, unsigned int order_number)
{
    this->log("Order [%d] %u was successfully prosecuted.\n", order_id, order_number);


    auto time = distr_accessTime_single(gen);
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

void OrdersManager::fake_save_on_db(const std::pair<int, unsigned int> orderList[], unsigned int size)
{
    for(int i=0; i<size; i++)
    {
        std::pair<int, unsigned int> order = orderList[i];
        this->log("Order [%d] %u was successfully prosecuted.\n", order.first, order.second);
    }

    auto time = distr_accessTime_multiple(gen);
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

void OrdersManager::process_orders()
{
    for(const auto& order: this->orders)
    {
        this->custom_fake_save_on_db(order.first, order.second);
        ++this->orders_processed;

        auto currentTime = std::chrono::system_clock::now();
        if (currentTime > this->last_printed_log)
        {
            this->last_printed_log = currentTime + std::chrono::seconds(5);
            log("Total orders executed: %u / %u\n", this->orders_processed, this->quantity);
        }
    }
}
long OrdersManager::runOrdersManager(OrdersManager *om, unsigned int quantity)
{

    auto startTime = std::chrono::system_clock::now();
    om->generate_fake_orders(quantity);
    om->startOrderManager();
    om->waitAndCleanOrderManager();
    auto endTime = std::chrono::system_clock::now();

    auto delay = std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count();

    om->log("Execution time: %ld\n", delay);

    return delay;
}

void OrdersManager::startOrderManager()
{
    this->process_orders();
}


void OrdersManager::waitAndCleanOrderManager()
{

}

