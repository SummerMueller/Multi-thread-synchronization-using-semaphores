# Multi-thread-synchronization-using-semaphores

## Problem 1
![Problem 1 terminal output](Screenshot%202025-11-17%20205710.png)
The no-starve readers-writers solution utilizes a switch and turnstile to allow the writers the opportunity to write. Without these important additions, its possible for the readers to read continuously and prevent the room from being empty which starves the writers from ever getting the chance to enter and write.

## Problem 2
![Problem 2 terminal output](Screenshot%202025-11-21%20161908.png)
In some cases, it may be desireable to give priority to the readers so they can make updates and prevent readers from accessing old data. To create the writer priority, two switches are utilized: one for the readers and one for the writers. When a writer is in the critical section, it allows only other writers to queue which gives them priority.

## Problem 3

## Problem 4
