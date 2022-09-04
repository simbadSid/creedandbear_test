
#include "../sharedTestRoutine.h"
#include "ordersManager_3_sharedList_BufferedList.h"





int main()
{
    OrdersManager *om = new OrdersManager_3_sharedList_BufferedList();

    bool res = simulateForEachSize(om, "OrdersManager_3_sharedList_BufferedList");

	if (res)
        return 0;
    else
        return -1;
}
