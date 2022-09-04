#include <list>

#include "ordersManager.h"


#ifndef TEST_SHAREDTESTROUTINE_H
#define TEST_SHAREDTESTROUTINE_H


std::vector<unsigned int> QUANTITY_LIST = std::vector<unsigned int>({10, 15, 20, 30, 50, 100, 250});//, 15, 20, 30, 50, 100, 250, 500, 750, 1000});


bool simulateForEachSize(OrdersManager *om, const std::string& className)
{
    std::cout << std::endl << "Class: " << className << ": ";

    for(const auto& quantity: QUANTITY_LIST)
    {
        long executionTime = OrdersManager::runOrdersManager(om, quantity);
        std::cout << executionTime << ", " ;
        bool res = om->isCorrectOrdersProcessing();
        if (!res)
        {
            std::cout << std::endl << "### Test failed for quantity = " << quantity << std::endl;
            return false;
        }
        fflush (stdout);
    }

    std::cout << std::endl;
    std::cout << "Test successful" << std::endl;

    return true;
}


#endif //TEST_SHAREDTESTROUTINE_H
