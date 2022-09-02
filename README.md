# creedandbear_test

## Proposed algorithms
This repository contains my XXXXXXXX.

In this context, we have designed and implemented 5 different algorithms to optimize the pattern followed to store the considered data. 
This algorithms (sorted from the less to the most complex/efficients) are:
* __0_Base__: the base algorithm proposed in the assignment.
* __1_bufferedDb__: the database is not accessed at each iteration.
Instead, the orders are stored within a local buffer and the database is only requested once the buffer is full.
* __2_sharedList__: this algorithm follows the model of the __single producer and multiple consumers__.
A unique thread is responsible for processing the orders and storing them within a shared list.
Different consumer threads are then responsible for parsing these orders and requesting the database. 
* __3_sharedList_BufferedList__: this algorithm uses the same architecture as the SimpleSharedList.
In addition, the producer thread reduces its latency by not accessing the shared list at each iteration.
Hence, it will reduce its access to the critical section (locked section).

* __4_sharedList_BufferedList_BufferedDb__: XXXXXXX
* __5_perThreadList_BufferedList_BufferedDb__: XXXXXXX

In the __Conclusion__ section, we present and explain our choice of algorithm for solving the considered problem.


## Algorithmic complexity
In this section, we will present the algorithmic complexity of the different algorithms in terms of time to access the database.
We also use __N__ as the number of orders to store.
* __Base__: This algorithm accesses sequentially to the database. Thus, its complexity is:
    ```
    O(N)
    ```
* __1_bufferedDb__:


An experimental evaluation of the proposed algorithm is plotted in XXXXXXX ![alt text](https://github.com/[username]/[reponame]/blob/[branch]/image.jpg?raw=true)


## Experimental benchmark
Inorder to compare the different proposed algorithm, we have implemented an experimental benchmarking (code XXXXXX).
The result of this benchmark are presented in XXXX.
This experimentation compares each algorithm using different number of orders.

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
cd bin
cmake ../
make;
```

Building the project results in generating the __bin/testOrdersManager__ executable file.
This file benchmarks the different implemented algorithm using different quantity of orders.