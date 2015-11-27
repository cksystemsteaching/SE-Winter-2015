Home Work Flow
--------------

* Step 0: form a team of 2-3 members
* Step 1: get a github account (for each member)
* Step 2: one person per group forks the SE-Winter-2015 repository by clicking [here](https://github.com/cksystemsteaching/SE-Winter-2015/fork) and adds the other team members as collaborators
* Step 3: check out the branch named __selfie-master__ in __your__ fork of SE-Winter-2015
* Step 4: implement the first assignment (see below)
* Step 5: add your names to the AUTHORS file
* Step 6: send a pull request containing your solution via github.com to [cksystemsteaching/SE-Winter-2015/tree/selfie-master](https://github.com/cksystemsteaching/SE-Winter-2015/tree/selfie-master)


Assignment 0: Basic data structures
-----------------------------------

Review [linked lists](https://en.wikipedia.org/wiki/Linked_list) and implement a simple program using a singly linked list in C*. The minimal requirements are as follows:

* must be implemented in C*
* must compile with selfie
* must run on selfie
* the list must be dynamically allocated
* every node must be dynamically allocated
* inserting nodes to the list and removing nodes from the list
* list iteration
* Bonus: sort the list. Any way you like
* Deadline: Oct 15, end of day


Assignment 1: Loading, scheduling, switching, execution
-------------------------------------------------------

Implement basic concurrent execution of _n_ processes in mipster. _n >= 2_ 

* understand how mipster [interprets and executes binary instructions](https://github.com/cksystemsteaching/SE-Winter-2015/blob/selfie-master/selfie.c#L3933). Tipp: add your own comments to the code
* mipster maintains a local state for a process (running executable), e.g., pc, registers, memory
* understand the purpose of each variable and data structure
* duplicate the process state n times
* running mipster like: _./selfie -m 32 yourbinary_ should generate _n_ instances of _yourbinary_ in a single instance of mipster
* implement [preemptive multitasking](https://en.wikipedia.org/wiki/Preemption_(computing)), i.e., switching between the _n_ instances of _yourbinary_ is determined by mipster 
* switch processes every m instructions. _1 <= m <= number of instructions in yourbinary_
* implement [round-robin scheduling](https://en.wikipedia.org/wiki/Round-robin_scheduling)
* add some output in _yourbinary_ to demonstrate context switching
* Deadline: Oct 22, end of day


Assignment 2: Memory segmentation, yield system call
----------------------------------------------------

This assignment deals with cooperative multitasking of _n_ processes in mipster using a single instance of physical memory.

* again, duplicate the process state _n_ times
* but, do not duplicate the whole main memory
* instead, split the main memory into segments by implementing a segment table in mipster
* each process has an entry in the segment table for the segment start address and segment size
* design the segment table for constant time access
* translate the addresses of read and write operations to memory

* implement [cooperative multitasking](https://en.wikipedia.org/wiki/Computer_multitasking) through a yield system call, i.e., a user process calling [sched_yield()](http://linux.die.net/man/2/sched_yield) will cause the OS to re-schedule
* implement a simple user program that demonstrates yielding, e.g, yield each time after printing a counter to the console
* Deadline: Oct 29, end of day


Assignment 3: Mutual Exclusion
------------------------------

Build this assignment on top of Assignment 2. Make sure that segmentation of memory works correctly.

* implement a single global lock through mipster syscalls, e.g., a lock() and unlock() call.
* implement a simple user program that demonstrates mutual exclusion, e.g, show that one process inside the critical section makes progress, processes not taking the lock make progress, and processes waiting for the lock do not make progress. Hint: you can implement the [getpid](http://linux.die.net/man/2/getpid) system call to identify processes.
* experiment with and demonstrate different interleavings: using locks, no locks, different time slices
* Deadline: Nov 5, end of day

* Bonus: implement basic multi-threading support
 * Idea: threads share one address space, processes don't
 * when duplicating processes, create threads instead, i.e., shared code, heap, globals, but private call stacks, private PC, private registers


Assignment 4: Fork and Wait
---------------------------

From now on, processes will trigger process creation and synchronization.

* Remove the process duplication code from Assignment 1 from selfie
* Implement the [fork](http://linux.die.net/man/2/fork) and [wait](http://linux.die.net/man/2/wait) system calls
* Read the man pages carefully to understand the semantics of both calls
* Modify the exit call such that terminating childprocesses do not quit the emulator
* Provide a demo process forking at least one child process that performs a different task along with the parent process. Play with interleavings and locks between parent and children
* Deadline: Nov 12, end of day

Assignment 5: Virtual Memory
----------------------------

* implement on-demand paging instead of memory segmentation in the emulator.
* partition the emulated memory (like physical memory on a real machine) into 4KB frames.
* each process gets a 4MB virtual address space, i.e., each process may access any address between 0x0 to 0x3FFFFFF. Virtual address space is organised in 4KB pages
* whenever a process actually accesses an address, allocate a frame and provide a mapping between pages and frames, i.e., a page table for each process
* modify the tlb function accordingly
* provide a demo program that allocates the whole virtual memory space but only accesses, for example, 128 different addresses evenly distributed within that region. Make sure that in this case only 128 frames are actually allocated, not the whole 4MB.
* page replacement and swapping is not required in this assignment

* Deadline: November 26, end of day


Assignment 6 + 7: Towards Mikro Kernels
---------------------------------------

1. In the emulator implement a microkernel that supports context and page table programming. You will need at least create_context, switch_context, delete_context, map_page_in_context, and flush_page_in_context. We distinguish these five calls into the microkernel from all other syscalls and call them hypercalls. create_context returns a unique ID to identify the new context, switch_context switches contexts, delete_context deletes contexts and recycles IDs, map_page_in_context creates a mapping for a virtual page, and flush_page_in_context unmaps it again. The key simplification is that the microkernel maintains contexts and page tables in the emulator. All processes are now in virtual memory including the OS.

Deadline: December 10, end of day


Until Dec 17 implement also:

2. On top of the emulator your OS in a single process that manages user processes as before but uses the microkernel in the emulator for switching. This process should have all of its virtual memory mapped 1-to-1 to physical memory. For paging the OS process may maintain its own copies of user page tables (in addition to the page tables in the microkernel) and also the free-list of page frames (most of the heap part of the virtual pages of the OS process become the page frames of user processes). This way the OS can read and write all user process memory, e.g. for loading code. Port the syscalls for your OS (fork, wait, lock, etc.) such that they lead to a "trap" into the microkernel which then switches contexts to the OS process to handle them. For this purpose use the $v0-v1 and $a0-a3 registers as before to have the OS process figure out what to do.

3. Similarly, all other events (timer interrupts, page faults) should also lead to "traps" into the microkernel which then make the microkernel also switch contexts to the OS process to handle them. Make sure that the OS process always runs to completion and is not interrupted by anything. This means the OS process sees something to do whenever it returns from context switching to a user process.

