# creedandbear_test

## Proposed algorithms

In this project, we have designed and implemented 5 different algorithms to optimize the pattern followed to store the considered data.
In the __Conclusion__ section, we present and explain our choice of algorithm for solving the considered problem.

The proposed algorithms (sorted from the less to the most complex/efficients) are:
* [0_Base](https://github.com/simbadSid/creedandbear_test/blob/main/code/src/ordersManager_0_base.cpp): the base algorithm proposed in the assignment.
* [1_bufferedDb](https://github.com/simbadSid/creedandbear_test/blob/main/code/src/ordersManager_1_bufferedDb.cpp): in this algorithm the database is not accessed at each iteration.
Instead, the orders are stored within a local buffer and the database is only requested once the buffer is full.
* [2_sharedList](https://github.com/simbadSid/creedandbear_test/blob/main/code/src/ordersManager_2_sharedList.cpp): this algorithm follows the model of the "single producer and multiple consumers".
A unique thread is responsible for processing of all the orders and storing them within a shared list.
Different consumer threads are then responsible for parsing these orders and requesting the database. 
Storing an order is then assynchrounous.
* [3_sharedList_BufferedList](https://github.com/simbadSid/creedandbear_test/blob/main/code/src/ordersManager_3_sharedList_BufferedList.cpp): this algorithm uses the same architecture as the SimpleSharedList.
In addition, the producer thread reduces its latency by not accessing the shared list at each iteration (buffers the orders until the buffer is full).
Hence, the will reduce the number of access to the critical section (locked section).
* [4_sharedList_BufferedList_BufferedDb](https://github.com/simbadSid/creedandbear_test/blob/main/code/src/ordersManager_4_sharedList_BufferedList_BufferedDb.cpp): in addition to the previous algorithm, this algorithm buffers the accesses to the daa base.
* [5_perThreadList_BufferedList_BufferedDb](https://github.com/simbadSid/creedandbear_test/blob/main/code/src/ordersManager_5_perThreadList_BufferedList_BufferedDb.cpp): in addition to the previous algorithm, this version implements a shared list for each consumer thread.
By storing the data within the thread that consumes them, this final optimization minimize drastically the number of accesses to the critical section.
The architecture of this algorithm is presented in the following graph.
![alt text](https://github.com/simbadSid/creedandbear_test/blob/main/resource/algorithm_5_architecture.drawio.png)



## Algorithmic complexity
In this section, we will present the algorithmic complexity of the different algorithms in terms of time to access the database.
We also use __N__ as the number of orders to store.
The algorithmic complexity of our algorithms is presented in the following figure. ![alt text](https://github.com/simbadSid/creedandbear_test/blob/main/resource/algorithmicComplexity.drawio.png)

* __0_Base__: This algorithm accesses sequentially to the database. Thus, its complexity is purely linear.
* __1_bufferedDb__: By reducing the number of accesses to the database (by a factor 1/Buffer Size), we reduce the complexity by the same factor.
* __2_sharedList__: Given that the accesses to the database are processed in parallel, we divide the complexity by the number of threads.
However, this is a theoretical complexity. In fact, the execution time is not divide by the number of threads. This is mainly due to the synchronization between threads.
* __3_sharedList_BufferedList__: this complexity is the same as the previous one. Indeed, the only thread that is optimized is the main thread which was already the fastest one.
* __4_sharedList_BufferedList_BufferedDb__: By buffering the accesses to the database, we divide the complexity of each consumer thread by the size of a buffer.
* __5_perThreadList_BufferedList_BufferedDb__: The complexity of this algorithm is the same as the previous one. However, this algorithm reduces the contention between the producer thread and the consumer threads. It also remove any synchroniation between the consumer threads.


## Experimental benchmark
Inorder to compare the different proposed algorithm, we have implemented an experimental benchmarking (code [link](https://github.com/simbadSid/creedandbear_test/blob/main/code/src/testOrdersManager.cpp)).
The result of this benchmark are presented in the following graphs.
![alt text](https://github.com/simbadSid/creedandbear_test/blob/main/resource/benchmark.png)
![alt text](https://github.com/simbadSid/creedandbear_test/blob/main/resource/benchmark_log.png)

This benchmark evaluates each algorithm using different number of orders.

It is noteworthy that our experimentation uses the following assumptions:
* For the sake of simplicity, the maximum latency to store an order within the database has been reduced to 10ms (instead of 1000ms in the original implementation).
* The maximum latency to store a relatively small set of orders (5) within the database is twice the time to store a single order.
* The database might be accessed by 5 concurrent thread with a latency similar to a single-thread access.
We have limited the number of concurrent thread to 5 inorder to keep this hypothesis true. 


## Conclusion
Even though the complexity of the different algorithms is roughly linear, the experimental benchmarks show a high difference in terms of execution time.

Indeed, the performance-critical part of our problem is the accesses to the database.
All the proposed algorithm have reduced the impact of database access by either reducing the number of accesses or by running them in parallel.

Given that the different orders are independent, the problem may clearly benefit from parallel execution.
The main issue is then to reduce the contention (synchronization) between concurrent run.
In this context, the __5_perThreadList_BufferedList_BufferedDb__ algorithm optimizes the need to synchronization by improving the locality of the data.
By creating a shared list local to each consumer thread, the algorithm needs no synchronization between consumer threads.

Consequently, our choice to solve the considered problem is the __5_perThreadList_BufferedList_BufferedDb__ algorithm.

It is noteworthy that our choice is only valid for a relatively large number of orders (> 50).
Otherwise, the time spent to create the different threads and to split the data becomes higher than the gain brought by our optimization.
In the case of a small number of orders, we would rather choose __1_bufferedDb__ which simply reduces the number of access to the database.

## Build
This project uses the CMake framework.

In order to build the project, we need to run the following command
```
mkdir bin
cd bin
cmake ../
make;
```

Building the project results in generating the __bin/testOrdersManager__ executable file.
This file benchmarks the different implemented algorithm using different quantity of orders.