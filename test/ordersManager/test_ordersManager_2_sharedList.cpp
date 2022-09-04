
#include "../sharedTestRoutine.h"
#include "ordersManager_2_sharedList.h"





int main()
{
    OrdersManager *om = new OrdersManager_2_sharedList();

    bool res = simulateForEachSize(om, "OrdersManager_2_sharedList");

    if (res)
        return 0;
    else
        return -1;
}
