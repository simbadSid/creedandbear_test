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
    std::srand(42);
}

void OrdersManager::generate_fake_orders(unsigned int quantity)
{
    log("\n\nGenerating fake orders: %u \n", quantity);

    this->orders = std::vector<Order>(quantity);
    this->orders_processed = 0;
    this->last_printed_log = std::chrono::system_clock::now();
    this->quantity = quantity;
    #ifdef TEST
    this->ordersinDB = std::vector<Order>(quantity);
    this->ordersinDBIndex = 0;
    #endif

    for (unsigned int i=0; i<this->quantity; ++i)
    {
        Order order(std::rand(), i);
        this->orders[i] = order;
    }

    log("%u generated...\n", this->quantity);
}

#ifdef LOG
void log(const char *format, ...)
{
//TODO ad date time in logger

    va_list ap;
    va_start (ap, format);
    vfprintf (stdout, format, ap);
    va_end (ap);
    fflush (stdout);
}
#endif

void OrdersManager::fake_save_on_db(Order order)
{
    log("Order [%d] %u was successfully prosecuted.\n", order.id, order.number);

    // Store the orders to be checked (unit test)
    #ifdef TEST
    {
        std::unique_lock<std::mutex> lockGuard(this->mtx);
        this->ordersinDB[this->ordersinDBIndex] = order;
        ++ this->ordersinDBIndex;
    }
    #endif

    ++order.id;
    auto time = distr_accessTime_single(gen);
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

void OrdersManager::fake_save_on_db(const Order orderList[], unsigned int size)
{
    for(unsigned int i=0; i<size; i++)
    {
        log("Order [%d] %u was successfully prosecuted.\n", orderList[i].id, orderList[i].number);
    }

    // Store the orders to be checked (unit test)
    #ifdef TEST
    {
        std::unique_lock<std::mutex> lockGuard(this->mtx);
        for(unsigned int i=0; i<size; i++)
        {
            log("Order [%d] %u was successfully prosecuted.\n", orderList[i].id, orderList[i].number);
            this->ordersinDB[this->ordersinDBIndex] = orderList[i];
            ++ this->ordersinDBIndex;
        }
    }
    #endif

    auto time = distr_accessTime_multiple(gen);
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
    orderList++;
}

void OrdersManager::process_orders()
{
    for(Order & order: this->orders)
    {
        this->custom_fake_save_on_db(order);
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

    log("Execution time: %ld\n", delay);

#ifdef TEST
    if (!om->isCorrectOrdersProcessing())
        throw std::invalid_argument("Wrong result of runOrderManager");
#endif

    return delay;
}

void OrdersManager::startOrderManager()
{
    this->process_orders();
}

void OrdersManager::waitAndCleanOrderManager()
{

}

#ifdef TEST
bool OrdersManager::isCorrectOrdersProcessing()
{
    if (this->ordersinDBIndex != this->quantity)
        return false;

    std::list<Order> allOrders = std::list<Order>(this->orders.begin(), this->orders.end());
    for (Order order : this->ordersinDB)
    {
        std::list<Order>::iterator it = std::find(allOrders.begin(), allOrders.end(), order);
        if (it == allOrders.end())
            return false;
        allOrders.erase(it);
    }

    if (!allOrders.empty())
        return false;

    return true;
}
#endif

