# creedandbear_test

# Proposed algorithms
This repository contains my XXXXXXXX.
In this context, we have designed and implemented 4 different algorithms to optimize the patter to store the considered data. 
This algorithms (sorted from the less to the most complex/efficients) are:
0. Base: the base algorithm proposed in the assignment.
1. BufferedSave: the database is not accessed at each iteration.
Instead, the orders are stored within a local buffer and the database is only requested once the buffer is full.
2. SimpleSharedList: this algorithm follows the model of the __single producer and multiple consumers__.
A unique thread is responsible for processing the orders and storing them within a shared list.
Different consumer threads are then responsible for parsing these orders and requesting the database. 
3. BufferedWriteSharedList: this algorithm uses the same architecture as the SimpleSharedList.
In addition, the producer thread reduces its lattency by not accessing the shared list at each itteration.
Hence, it will reduce its access to the critical section (locked section).

## Algorithmic complexity
XXXXX

An experimental evaluation of the proposed algorithm is ploted in the resource directory

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