# Multi-thread-synchronization-using-semaphores

## Problem 1
![Problem 1 terminal output](Screenshot%202025-11-17%20205710.png)
The no-starve readers-writers solution utilizes a switch and turnstile to allow the writers the opportunity to write. Without these important additions, its possible for the readers to read continuously and prevent the room from being empty which starves the writers from ever getting the chance to enter and write.

## Problem 2
![Problem 2 terminal output](Screenshot%202025-11-21%20161908.png)
In some cases, it may be desireable to give priority to the readers so they can make updates and prevent readers from accessing old data. To create the writer priority, two switches are utilized: one for the readers and one for the writers. When a writer is in the critical section, it allows only other writers to queue which gives them priority.

## Problem 3
![Problem 3 terminal output](Screenshot%202025-11-24%20194739.png)
Allowing one less philosopher than the amount of forks at the table will validate that there is never a deadlock. To simulate this, a "footman" semaphore is created to ensure that a philosopher can't ever wait on a fork if four poeple are already at the table. Due to the footman, this solution also prevents starvation.

## Problem 4
![Problem 4 terminal output](Screenshot%202025-11-24%20203414.png)
The other way to ensure that there is never a deadlock in the dining philosophers problem is to force there to be at least one leftie and at least one rightie at the table. In my implementation, all even philosophers are righties and the odd philosophers are lefties. This solution also prevents starvation.
