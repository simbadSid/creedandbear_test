
#include "../sharedTestRoutine.h"
#include "ordersManager_0_base.h"





int main()
{
    OrdersManager *om = new OrdersManager_0_base();

    bool res = simulateForEachSize(om, "OrdersManager_0_base");

    if (res)
        return 0;
    else
        return -1;
}
