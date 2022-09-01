#include <list>

#include "OrdersManager_0_base.h"
#include "OrdersManager_1_bufferedSave.h"
#include "OrdersManager_2_simpleSharedList.h"
#include "OrdersManager_3_bufferedWriteSharedList.h"

//std::vector<int> QUANTITY_LIST = std::vector<int>({10, 20, 50, 100, 250, 500, 750, 1000});
std::vector<unsigned int> QUANTITY_LIST = std::vector<unsigned int>({10});//, 20, 50, 100, 250, 500, 750, 1000});


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
//            std::pair<OrdersManager*, std::string> (new OrdersManager_0_base(),             "OrdersManager_0_base"),
//            std::pair<OrdersManager*, std::string> (new OrdersManager_1_bufferedSave(),     "OrdersManager_1_bufferedSave"),
//            std::pair<OrdersManager*, std::string> (new OrdersManager_2_simpleSharedList(), "OrdersManager_2_simpleSharedList"),
            std::pair<OrdersManager*, std::string> (new OrdersManager_3_bufferedWriteSharedList(), "OrdersManager_3_bufferedWriteSharedList")
    };

    for (const auto& c : l)
    {
        simulateForEachSize(c.first, c.second);
        delete c.first;
    }
}
