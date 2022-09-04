
#include "../sharedTestRoutine.h"
#include "ordersManager_1_bufferedDb.h"





int main()
{
    OrdersManager *om = new OrdersManager_1_bufferedDb();

    bool res = simulateForEachSize(om, "OrdersManager_1_bufferedDb");

    if (res)
        return 0;
    else
        return -1;
}
