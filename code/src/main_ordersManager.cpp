#include <list>

#include "ordersManager_0_base.h"
#include "ordersManager_1_bufferedDb.h"
#include "ordersManager_2_sharedList.h"
#include "ordersManager_3_sharedList_BufferedList.h"
#include "ordersManager_4_sharedList_BufferedList_BufferedDb.h"
#include "ordersManager_5_perThreadList_BufferedList_BufferedDb.h"

//std::vector<int> QUANTITY_LIST = std::vector<int>({10, 15, 20, 30, 50, 100, 150, 250, 500, 750, 1000, 1500, 3000, 5000, 10000, 50000,}); //, 100000, 500000, 1000000, 10000000});
std::vector<unsigned int> QUANTITY_LIST = std::vector<unsigned int>({10, 15, 20, 30, 50, 100, 250});//, 15, 20, 30, 50, 100, 250, 500, 750, 1000});


void simulateForEachSize(OrdersManager *om, const std::string& className)
{
    std::cout << std::endl << "Class: " << className << ": ";

    for(const auto& quantity: QUANTITY_LIST)
    {
        long executionTime = OrdersManager::runOrdersManager(om, quantity);
        std::cout << executionTime << ", " ;
        fflush (stdout);
    }
    std::cout << std::endl;
}
int main()
{
    std::list<std::pair<OrdersManager*, std::string> >l = {
            std::pair<OrdersManager*, std::string> (new OrdersManager_0_base(),                                   "OrdersManager_0_base"),
            std::pair<OrdersManager*, std::string> (new OrdersManager_1_bufferedDb(),                             "OrdersManager_1_bufferedDb"),
            std::pair<OrdersManager*, std::string> (new OrdersManager_2_sharedList(),                             "OrdersManager_2_sharedList"),
            std::pair<OrdersManager*, std::string> (new OrdersManager_3_sharedList_BufferedList(),                "OrdersManager_3_sharedList_BufferedList"),
            std::pair<OrdersManager*, std::string> (new OrdersManager_4_sharedList_BufferedList_BufferedDb(),     "OrdersManager_4_sharedList_BufferedList_BufferedDb"),
            std::pair<OrdersManager*, std::string> (new OrdersManager_5_perThreadList_BufferedList_BufferedDb(),"OrdersManager_5_perThreadList_BufferedList_BufferedDb")
    };

    for (const auto& c : l)
    {
        simulateForEachSize(c.first, c.second);
        delete c.first;
    }
}
