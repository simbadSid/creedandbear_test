
#include "../sharedTestRoutine.h"
#include "ordersManager_4_sharedList_BufferedList_BufferedDb.h"





int main()
{
    OrdersManager *om = new OrdersManager_4_sharedList_BufferedList_BufferedDb();

    bool res = simulateForEachSize(om, "OrdersManager_4_sharedList_BufferedList_BufferedDb");

    if (res)
        return 0;
    else
        return -1;
}
