//
// Created by Riyane on 31/08/2022.
//

#include "ordersManager.h"


OrdersManager::OrdersManager(unsigned int quantity)
{
    this->orders = std::vector<std::pair<int, unsigned int> >(quantity);
    this->orders_processed = 0;
    this->last_printed_log = std::chrono::system_clock::now();
    this->quantity = quantity;

    // Use a seed for the random generator
    std::srand(42);

    this->generate_fake_orders();
}

void OrdersManager::generate_fake_orders()
{
    this->log("Generating fake orders: %u\n", this->quantity);

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

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, 1000);

    auto time = distr(gen);
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

void OrdersManager::process_orders()
{

//TODO pb in loop
    for(const auto& order: this->orders)
    {
        this->custom_fake_save_on_db(order.first, order.second);
        ++this->orders_processed;

        auto currentTime = std::chrono::system_clock::now();
        if (currentTime > this->last_printed_log)
        {
            this->last_printed_log = currentTime + std::chrono::seconds(5);
            log("Total orders executed: %d\n", this->orders_processed/this->quantity);
        }
    }
}


void OrdersManager::runOrdersManager(OrdersManager *om)
{
    auto startTime = std::chrono::system_clock::now();
    om->process_orders();
    auto delay = std::chrono::system_clock::now() - startTime;

//TODO print delay
    om->log("Execution time: ");
}
