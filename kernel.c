// timer, pager, or syscall
int whichServer;

// ------ user registers (data for syscalls) ------
int user_V0;
int user_A0;
int user_A1;
int user_A2;
int user_A3;

// ------ register offsets ------
int REG_ZR = 0;
int REG_AT = 1;
int REG_V0 = 2;
int REG_V1 = 3;
int REG_A0 = 4;
int REG_A1 = 5;
int REG_A2 = 6;
int REG_A3 = 7;
int REG_T0 = 8;
int REG_T1 = 9;
int REG_T2 = 10;
int REG_T3 = 11;
int REG_T4 = 12;
int REG_T5 = 13;
int REG_T6 = 14;
int REG_T7 = 15;
int REG_S0 = 16;
int REG_S1 = 17;
int REG_S2 = 18;
int REG_S3 = 19;
int REG_S4 = 20;
int REG_S5 = 21;
int REG_S6 = 22;
int REG_S7 = 23;
int REG_T8 = 24;
int REG_T9 = 25;
int REG_K0 = 26;
int REG_K1 = 27;
int REG_GP = 28;
int REG_SP = 29;
int REG_FP = 30;
int REG_RA = 31;
int REG_HI = 32;
int REG_LO = 33;

// ------ server ------
int OS_TIMER = 5;
int OS_PAGER = 6;
int OS_SYSCALL = 7;

// ------ syscalls ------
int SYSCALL_EXIT    = 4001;
int SYSCALL_FORK    = 4002;
int SYSCALL_WAIT    = 4007;
int SYSCALL_GETPID  = 4020;
int SYSCALL_YIELD   = 4162;
int SYSCALL_LOCK    = 6000;
int SYSCALL_UNLOCK  = 6001;
int SYSCALL_MALLOC  = 5001;
int SYSCALL_EXEC    = 7000;

// ------ processes ------
int pid = 0;  // last process identifier
int lock = 0;

// ------ queues ------
int *readyQ;
int *blockedQ;
int *zombieQ;
int *waitQ;

// ------ memory ------
int  *pageFrameRefs;
int  virtualMemorySize = 4194304; //4*1024*1024
int  pageSize = 4096;             // pageSize in bytes
int  maxPages;                    // maxium of pages
int  allocatedPages = 0;
int  freep = 0;

int *userSpace;
int  bumpPtr;
int *binaryName;
int binaryLength = 0;

int invalidPage = 0;
int readOnlyPage = 1;
int readAndWritePage = 2;

// ------ debugging ------
int debug_getpid  = 0;
int debug_yield   = 0;
int debug_lock    = 0;
int debug_unlock  = 0;
int debug_exit    = 1;
int debug_fork    = 0;
int debug_wait    = 0;
int debug_malloc  = 0;
int debug_exec    = 0;
int debug_pr      = 0;


// ------ library ------
int CHAR_EOF          = -1; // end of file
int CHAR_TAB          = 9;  // ASCII code 9  = tabulator
int CHAR_LF           = 10; // ASCII code 10 = line feed
int CHAR_CR           = 13; // ASCII code 13 = carriage return
int CHAR_SPACE        = ' ';
int CHAR_SEMICOLON    = ';';
int CHAR_PLUS         = '+';
int CHAR_DASH         = '-';
int CHAR_ASTERISK     = '*';
int CHAR_HASH         = '#';
int CHAR_SLASH        = '/';
int CHAR_UNDERSCORE   = '_';
int CHAR_EQUAL        = '=';
int CHAR_LPARENTHESIS = '(';
int CHAR_RPARENTHESIS = ')';
int CHAR_LBRACE       = '{';
int CHAR_RBRACE       = '}';
int CHAR_COMMA        = ',';
int CHAR_LT           = '<';
int CHAR_GT           = '>';
int CHAR_EXCLAMATION  = '!';
int CHAR_PERCENTAGE   = '%';
int CHAR_SINGLEQUOTE  = 39; // ASCII code 39 = '
int CHAR_DOUBLEQUOTE  = '"';

int *power_of_two_table;

int INT_MAX; // maximum numerical value of an integer
int INT_MIN; // minimum numerical value of an integer

int *string_buffer;

int O_RDONLY = 0;


// ---------------------------------------------
// ----------------- FUNCTIONS -----------------
// ---------------------------------------------

// ------ initialization ------
void init(int argc, int* argv, int kernelBP);
void loadFirstUserProcess(int *argv);
void load(int *process, int* path);
int  findFrameForLoad(int* process, int vpn);
void initRegisterPointers(int* process);
int* kmalloc(int size);

// ------ server ------
int server_timer();
int server_pager();
int server_syscall();

// ------ syscalls ------
void syscall_exit();
void syscall_fork();
void syscall_getpid();
void syscall_yield();
void syscall_lock();
void syscall_unlock();
void syscall_malloc();
void syscall_wait();

// ------ paging ------
int  os_pageFaultHandler(int *process, int vpn);
int  palloc();
void pfree(int p);
void os_insertIntoPageTable(int* process, int vpn, int rpn, int mode);
void os_pageDaemon(int refCount, int rpn);
void os_updatePage(int *process, int vpn, int newRpn);
void os_copyPageFrame(int fromRpn, int toRpn);
int  os_getPageFlag(int* process, int vpn);
int  os_getRPNfromPageTable(int* process, int vpn);

// ------ queues ------
int* os_createQueue();
int* os_removeQueue(int* list, int* data);
int* os_getHead(int* list);
int* os_getTail(int* list);
int* os_appendQueue(int* list, int* data);

// ------ process queues ------
int* os_searchPqueue(int* list, int field, int key);
void os_printQueue(int* list, int* qname, int field);

// ------ process control block ------
int* os_createProcess();
int  os_getProcessID(int* process);
int  os_getProcessPc(int* process);
int* os_getProcessRegisters(int* process);
int  os_getProcessPageFault(int* process);
int  os_addProcessPageFault(int* process);
int* os_getProcessParent(int* process);
int  os_getProcessExitCode(int* process);
int  os_processTerminated(int* process);
int* os_getProcessPageTable(int* process);
int* os_getProcessPageTableFlags(int* process);

// ------ scheduling ------
void os_contextSwitch();
void os_fifo();

// ------ fork helper functions ------
void os_copyRegisters(int* fromProcess, int* toProcess);
void os_forkPageTable(int *process, int *new_process);
int  os_incPageFrameRefs(int rpn);
int  os_decPageFrameRefs(int rpn);

// ------ debugging ------
void printPageTable(int *process);
void printMemory();
void printRegisters(int* reg);

// ------ ĺibrary ------
void initLibrary();
int twoToThePowerOf(int p);
int leftShift(int n, int b);
int rightShift(int n, int b);
int  stringLength(int *s);
void stringReverse(int *s);
int  stringCompare(int *s, int *t);
int  atoi(int *s);
int* itoa(int n, int *s, int b, int a);
void print(int *s);
void println();
void printCharacter(int character);
void printString(int *s);

void exit(int code);
int* malloc(int size);


// ---------------------------------------------------------------------
// ---------------------------------------------------------------------
// ---------------------------------------------------------------------
int main(int argc, int *argv, int kernelBP) {
    bumpPtr = kernelBP;

    initLibrary();
    init(argc, argv, kernelBP);

    server_timer();


    while (1) {
       // print("server nr: ");
       // print((int*)itoa(whichServer,string_buffer,10,0));
       // println();
        if (whichServer == OS_TIMER) {
            server_timer();
        } else if (whichServer == OS_PAGER) {
            server_pager();
        } else if (whichServer == OS_SYSCALL) {
            server_syscall();
        } else {
            print((int*)"ERROR: wrong server");
            println();
        }
    }
}

// -----------------------------------------------------------------
// ------------------------- INITIALIZATION ------------------------
// -----------------------------------------------------------------

void init(int argc, int* argv, int kernelBP) {
    int kernelMemory;

    kernelMemory = atoi(*argv)*1024*1024;
    userSpace = (int*)(kernelMemory/2);
    binaryName = (int*)*(argv+1);

    maxPages = virtualMemorySize/pageSize;
    pageFrameRefs = kmalloc(4*((int)(userSpace/(int*)pageSize)));

    print("bumpPointer: ");
    print(itoa(kernelBP, string_buffer, 10, 0));
    print(", kernel Mem: ");
    print(itoa(kernelMemory, string_buffer, 10,0 ));
    print(", binaryName: ");
    print(binaryName);
    println();

    readyQ   = os_createQueue();
    blockedQ = os_createQueue();
    waitQ    = os_createQueue();
    zombieQ  = os_createQueue();

    loadFirstUserProcess();
}

void loadFirstUserProcess() {
    int *initProcess;

    initProcess = os_createProcess();
    os_appendQueue(readyQ, initProcess);

    load(initProcess, binaryName);
}

void load(int *process, int *path) {
    int fd;
    int numberOfReadBytes;
    int vpn;
    int pageOffset;
    int *address;
    int rpn;

    fd = open(path, O_RDONLY, 0);

    if (fd < 0) {
        print((int*) "Could not open input file ");
        print(path);
        println();
        //halt(-1);
    }

    //print((int*) "Loading code from input file ");
    //print(path);
    //println();

    vpn = 0;
    pageOffset = 0;
    numberOfReadBytes = 4;

    rpn = findFrameForLoad(process, vpn);

    while (numberOfReadBytes == 4) {
        address = userSpace + (rpn*pageSize + pageOffset)/4;
        numberOfReadBytes = read(fd, address, 4);

        pageOffset = pageOffset + 4;

        if (pageOffset == pageSize) {
            vpn = vpn + 1;
            pageOffset = 0;

            rpn = findFrameForLoad(process, vpn);
        }

        if (numberOfReadBytes == 4)
            binaryLength = binaryLength + 4;
    }

    initRegisterPointers(process);
}

int findFrameForLoad(int* process, int vpn) {
    int *pt_flags;
    int oldRpn;
    int newRpn;

    pt_flags = os_getProcessPageTableFlags(process);

    if (*(pt_flags + vpn) == invalidPage) {
        newRpn = os_pageFaultHandler(process, vpn);

    } else if (*(pt_flags + vpn) == readOnlyPage) {
        newRpn = palloc();
        oldRpn = os_getRPNfromPageTable(process, vpn);
        os_decPageFrameRefs(oldRpn);
        os_updatePage(process, vpn, newRpn);

    } else {
        print("readWritePage?");
        println();
    }

    return newRpn;
}

void initRegisterPointers(int* process) {
    int *process_regs;

    process_regs = os_getProcessRegisters(process);

    *(process_regs+REG_GP) = binaryLength;
    *(process_regs+REG_K1) = *(process_regs+REG_GP);
    *(process_regs+REG_SP) = virtualMemorySize;
}

int* kmalloc(int size) {
    int oldBumpPtr;

    if (size % 4 != 0)
        size = size + 4 - size % 4;

    oldBumpPtr = bumpPtr;
    bumpPtr = bumpPtr + size;

    return (int*)oldBumpPtr;
}

// -----------------------------------------------------------------
// ----------------------------- SERVER ----------------------------
// -----------------------------------------------------------------

int server_timer() {
    os_contextSwitch();
}

int server_pager() {
    int *process;
    int vpn;
    int newRpn;
    int oldRpn;
    int flag;

    process = os_getHead(readyQ);
    vpn = user_A0;
    flag = os_getPageFlag(process, vpn);

    if (flag == invalidPage) {
        os_pageFaultHandler(process, vpn);
    } else if (flag == readOnlyPage) {
        newRpn = palloc();
        oldRpn = os_getRPNfromPageTable(process, vpn);

        os_copyPageFrame(oldRpn, newRpn);
        os_pageDaemon(os_decPageFrameRefs(oldRpn), oldRpn);
        os_updatePage(process, vpn, newRpn);
    }
    os_contextSwitch();
}

int server_syscall() {
   // XXX print((int*)"syscall number ");
   // print((int*)itoa(user_V0,string_buffer,10,0));
   // println();

    if (user_V0 == SYSCALL_EXIT) {
        syscall_exit();
    } else if (user_V0== SYSCALL_FORK) {
        syscall_fork();
    } else if (user_V0 == SYSCALL_GETPID) {
        syscall_getpid();
    } else if (user_V0 == SYSCALL_YIELD) {
        syscall_yield();
    } else if (user_V0 == SYSCALL_LOCK) {
        syscall_lock();
    } else if (user_V0 == SYSCALL_UNLOCK) {
        syscall_unlock();
    } else if (user_V0 == SYSCALL_MALLOC) {
        syscall_malloc();
    } else if (user_V0 == SYSCALL_WAIT) {
        syscall_wait();
    } else if (user_V0 == SYSCALL_EXEC) {
        syscall_exec();
    } else {
        // unkown syscall
        halt(-2);
    }

    server_timer();
}


// -----------------------------------------------------------------
// --------------------------- SYSCALLS ----------------------------
// -----------------------------------------------------------------

void syscall_exit() {
    int exitCode;
    int* process;
    int* parent;
    int* parentRegs;
    int  i;
    int  rpn;
    int* pTable;
    int* pTableFlags;

    process = os_getHead(readyQ);
    pTable   = os_getProcessPageTable(process);
    pTableFlags = os_getProcessPageTableFlags(process);
    exitCode = user_A0;
    parent = os_getProcessParent(process);

    // free page table memory:
    i = 0;
    while (i < pageSize/4) {
        if (*(pTableFlags + i) != invalidPage) {
            rpn = *(pTable + i);
            os_pageDaemon(os_decPageFrameRefs(rpn), rpn);
        }
        i = i + 1;
    }

    if (debug_exit) {
        print((int*)"[OS] PID");
        print(itoa(os_getProcessID(process), string_buffer, 10, 0));
        print((int*)" Terminated with ");
        print(itoa(exitCode, string_buffer, 10, 0));
        println();
    }

    *(process+7) = exitCode;
    *(process+8) = 1; //has terminated

    os_removeQueue(readyQ, process);

    if ((int)parent != 0) {
        if ((int)os_searchPqueue(waitQ, 2, os_getProcessID(parent)) == 0) {
            if (os_processTerminated(parent))
                *(process+6) = 0;    //process gets adopted by init
            else {
                os_appendQueue(zombieQ, process);
                if (debug_pr)
                    os_printQueue(zombieQ, (int*)"[PR] Zombie Queue: ", 0);
            }
        } else {
            os_removeQueue(waitQ, parent);
            os_appendQueue(readyQ, parent);
            parentRegs = os_getProcessRegisters(parent);
            *(parentRegs+REG_V0) = exitCode;
        }
    }
}


void syscall_fork() {
    int *process;
    int *process_regs;
    int *new_process;
    int *new_process_regs;

    process = os_getHead(readyQ);
    new_process = os_createProcess();

    *(new_process+3) = os_getProcessPc(process);
    *(new_process+6) = (int)process;
    os_appendQueue(readyQ, new_process);

    process_regs = os_getProcessRegisters(process);
    new_process_regs = os_getProcessRegisters(new_process);

    os_copyRegisters(process, new_process);

    os_forkPageTable(process, new_process);

    // return values of fork for parent and child
    *(process_regs + REG_V0) = os_getProcessID(new_process);
    *(new_process_regs + REG_V0) = 0;


    if (debug_fork) {
        print((int*)"PID ");
        print(itoa(os_getProcessID(process),string_buffer,10,0));
        print((int*) ": forked process ");
        print(itoa(os_getProcessID(new_process), string_buffer, 10, 0));
        println();
    }
}


void syscall_getpid() {
    int *process;
    int *process_regs;

    process = os_getHead(readyQ);
    process_regs = os_getProcessRegisters(process);

    *(process_regs+REG_V0) = os_getProcessID(process);

    if (debug_getpid) {
        print((int*)"PID ");
        print(itoa(os_getProcessID(process),string_buffer,10,0));
        print((int*) ": getpid ");
        print(itoa(*(process_regs+REG_V0), string_buffer, 10, 0));
        print((int*) " from running process");
        println();
    }
}

void syscall_yield() {
    int* process;

    process = os_getHead(readyQ);

    if (debug_yield) {
         print((int*)"PID ");
        print(itoa(os_getProcessID(process),string_buffer,10,0));
        print((int*) ": yield");
        println();
    }
    os_removeQueue(readyQ, process);
    os_appendQueue(readyQ, process);
}

void syscall_lock() {
    int *process;
    int *process_regs;

    process = os_getHead(readyQ);
    process_regs = os_getProcessRegisters(process);

    if (lock == 0) {
        lock = os_getProcessID(process);
        *(process_regs+REG_V0) = 1;

    } else {
        *(process_regs+REG_V0) = 0;
        os_removeQueue(readyQ, process);
        os_appendQueue(blockedQ, process);
    }

    if (debug_lock) {
        os_printQueue(blockedQ, (int*)"[PR] Blocked Queue: ", 0);
    }

    if (debug_lock) {
         print((int*)"PID ");
        print(itoa(os_getProcessID(process),string_buffer,10,0));
        print((int*) ": lock set");
        println();
    }
}

void syscall_unlock() {
    int *process;
    int *process_regs;
    int *firstBlocked;

    process = os_getHead(readyQ);

    if (lock == os_getProcessID(process)) {
        lock = 0;
        *(process_regs+REG_V0) = 1;
        firstBlocked = os_getHead(blockedQ);

        if ((int) firstBlocked != 0) {
            os_removeQueue(blockedQ, firstBlocked);
            os_appendQueue(readyQ, firstBlocked);
            lock = os_getProcessID(firstBlocked);
        }  else
             *(process_regs+REG_V0) = 0;
    } else {
         *(process_regs+REG_V0) = 0;
          // exception.. kill process?
    }

    if (debug_unlock) {
        print((int*)"PID ");
        print(itoa(os_getProcessID(process),string_buffer,10,0));
        print((int*) ": blocked queue after unlock: ");
        os_printQueue(blockedQ, (int*)"[PR] Blocked Queue: ", 0);
        println();
    }

    if (debug_unlock) {
        print((int*)"PID ");
        print(itoa(os_getProcessID(process),string_buffer,10,0));
        print((int*) ": succesfully reset lock? : ");
        print(itoa(*(process_regs + REG_V0), string_buffer, 10, 0));
        println();
    }
}

void syscall_malloc() {
    int size;
    int bump;
    int *process;
    int *process_regs;

    process = os_getHead(readyQ);
    process_regs = os_getProcessRegisters(process);

    size = user_A0;

    if (size % 4 != 0)
        size = size + 4 - size % 4;

    bump = *(process_regs+REG_K1);

    if (debug_malloc) {
         print((int*)"PID ");
        print(itoa(os_getProcessID(process),string_buffer,10,0));
        print((int*) ": malloc ");
        print(itoa(size, string_buffer, 10, 0));
        print((int*) " bytes returning address ");
        print(itoa(bump, string_buffer, 16, 8));
        println();
    }

    if (bump + size >= *(process_regs+REG_SP))
        halt(-3);

    *(process_regs+REG_K1) = bump + size;
    *(process_regs+REG_V0) = bump;
}

void syscall_wait() {
    int *parent;
    int *parent_regs;
    int *process;
    int *process_regs;
    int pid;

    process_regs = os_getProcessRegisters(process);
    parent = os_getHead(readyQ);
    parent_regs = os_getProcessRegisters(parent);
    pid = user_A0;

    process = os_searchPqueue(readyQ, 2, pid);
    if ((int)process == 0) {
        process = os_searchPqueue(blockedQ, 2, pid);
        if ((int)process == 0) {
            process = os_searchPqueue(zombieQ, 2, pid);
            if ((int)process == 0) {
                return;            // nothing to wait for
            } else {
                *(parent_regs+REG_V0) = os_getProcessExitCode(process);
                os_removeQueue(zombieQ, process);

                if (debug_wait) {
                    print((int*)"PID ");
        print(itoa(os_getProcessID(process),string_buffer,10,0));
                    print((int*) " removed zombie ");
                    print(itoa(os_getProcessID(process), string_buffer, 10, 0));
                    println();
                }
                return;
            }
        } else {
            os_removeQueue(readyQ, parent);
            os_appendQueue(waitQ, parent);
            *(parent+9) = pid;
        }
    } else {
        os_removeQueue(readyQ, parent);
        os_appendQueue(waitQ, parent);
        *(parent+9) = pid;
    }

    if (debug_wait) {
        print((int*)"PID ");
        print(itoa(os_getProcessID(process),string_buffer,10,0));
        print((int*) " waits for child ");
        print(itoa(os_getProcessID(process), string_buffer, 10, 0));
        println();
    }

    if (debug_wait)
        os_printQueue(waitQ, (int*)"[PR] Wait Queue: ", 0);
}

void syscall_exec() {
    int *process;
    int *path;

    process = os_getHead(readyQ);

    path = (int*)user_A0;

    if (debug_exec) {
        print((int*)"PID ");
        print(itoa(os_getProcessID(process),string_buffer,10,0));
        print((int*) ": exec binary ");
        print((int*) path);
        println();
    }

    load(process, path);
    *(process+3) = 0;   // reset pc to 0

}

// -----------------------------------------------------------------
// ----------------------------- PAGING ----------------------------
// -----------------------------------------------------------------

// purpose: handle page faults for a given virtual page number "vpn":
//          increment the page fault counter, allocate a physical page
//          and insert this allocated page into the pagetable.
int os_pageFaultHandler(int *process, int vpn) {
    int rpn;
    //os_addProcessPageFault(process);

    rpn = palloc();
    os_insertIntoPageTable(process, vpn, rpn, readAndWritePage);
    //printPageTable(process);

    return rpn;
}

// purpose:  allocate new page from page-free list
int palloc() {
    int current;

    current = freep;

    if (allocatedPages+1 > maxPages) {
        // TODO page replacement
        return 0;
    }

    if (*(userSpace + freep*pageSize) != 0)
        freep = *(userSpace + freep*pageSize);
    else
        freep = freep + 1;

    os_incPageFrameRefs(current);
    allocatedPages = allocatedPages + 1;

    return current;
}

// purpose:  free a page p.
void pfree(int p) {
    // XXX set whole page to 0
    *(userSpace + p*pageSize) = freep;
    freep = p;

    allocatedPages = allocatedPages - 1;
}

// purpose: insert (vpn, rpn) into a page table of a process.
//          mode is used to specify if the page is read/write able
void os_insertIntoPageTable(int* process, int vpn, int rpn, int mode) {
    int* table;

    table = (int*)*(process + 10);
    *(table + vpn) = rpn;

    table = (int*)*(process + 11);
    *(table + vpn) = mode;
}

// purpose:  kernel process "pagedaemon", to free physical pages
//           that are no longer needed by any process.
void os_pageDaemon(int refCount, int rpn) {
    if (refCount == 0) {
        if(0) {
            print((int*)"free rpn: ");
            print((int*)itoa(rpn,string_buffer,10,0));
            println();
        }
        pfree(rpn);
    }
}

// purpose:  replace a virtual page number (vpn, oldRpn) with (vpn,neRpn)
//           Used for updating a page table entry.
void os_updatePage(int *process, int vpn, int newRpn) {
    int *ptable;
    int *ptableFlags;

    ptable = os_getProcessPageTable(process);
    ptableFlags = os_getProcessPageTableFlags(process);
    *(ptable + vpn) = newRpn;
    *(ptableFlags + vpn) = readAndWritePage;
}



// purpose:  copy physical page
void os_copyPageFrame(int fromRpn, int toRpn) {
    int fromPageFrame;
    int toPageFrame;
    int i;

    fromPageFrame = leftShift(fromRpn,12);
    toPageFrame = leftShift(toRpn,12);

    i = 0;
    while (i < pageSize) {
        *(userSpace + (leftShift(toRpn, 12) + i) / 4) = *(userSpace +(leftShift(fromRpn, 12) + i) / 4);
        i = i + 4;
    }
}

// purpose: return the page flag  table for a process
int os_getPageFlag(int* process, int vpn) {
    int* table;
    table = (int*)*(process + 11);

    return *(table + vpn);
}

// purpose: return real page number (rpn) for process and a given
//          virtual page number:  [ rpn -> vpn ]
int os_getRPNfromPageTable(int* process, int vpn) {
    int* table;

    table = (int*)*(process + 10);

    return *(table + vpn);
}

// -----------------------------------------------------------------
// ----------------------------- QUEUES ----------------------------
// -----------------------------------------------------------------

// purpose:  create and initialize a queue, return pointer to it.
//           a queue consists of a pointer to a head and a tail.
int* os_createQueue() {
    int *q;
    q = kmalloc(2*4);
    *(q + 0) = 0;             // head
    *(q + 1) = 0;             // tail
    return q;
}

// purpose:  remove element "data" from list
int* os_removeQueue(int* list, int* data) {
    int *prev;
    int *next;
    int *tmp;

    if (*list == 0) {
       return (int*)0;
    }

    if ((int)data == 0) {
        return (int*)0;
    }

    if ((int)data == *list) {      // case 1: we must remove the head
        // data is head (?)
        *list = *data;
        tmp = (int*)*list;

        if ((int)tmp != 0) {
            *(tmp+1) = 0;          // set prev from new head element
        }
        if ((int)data == *(list+1)) {
            *(list+1) = 0;
        }
    } else if ((int)data == *(list + 1)) {// case 2: we must remove the tail
        *(list+1) = *(data + 1);
        tmp = (int*)*(list + 1);
        *tmp = 0;
    } else {                       // case 3: we must remove in between.
        prev = (int*)*(data + 1);
        next = (int*)*(data + 0);
        *prev = (int)next;
        *(next + 1) = (int)prev;
    }
    return data;
}

// purpose: get head of list
int* os_getHead(int* list) {
    return (int*)*list;
}

// purpose: get tail of list
int* os_getTail(int* list) {
    return (int*)*(list+1);
}


// append:  append a data object 'data' to the list
int* os_appendQueue(int* list, int* data) {
    int* head;
    int* tail;
    int tmp;

    *data    = 0;                // next
    *(data+1)= 0;                // prev
    head = (int*)*(list + 0);
    tail = (int*)*(list + 1);

    if ((int)head == 0) {
        // list is empty!
        *list = (int)data;
        *(list + 1) = (int)data;
        return data;
    } else {
        tmp = (int)tail;
        *tail = (int)data;
        *(list+1) = (int)data;
        *(data+1) = (int)tmp;
    }
    return (int*)0;
}

// -----------------------------------------------------------------
// ------------------------- PROCESS QUEUES ------------------------
// -----------------------------------------------------------------

// purpose: search process queue, key is a PID
//          important: list must be a list of 'process'-records.
int* os_searchPqueue(int* list, int field, int key) {
    int* q;

    q = (int*)*list;

    while ((int)q != 0) {
        if (*(q + field) == key)
            return q;
        q = (int*)*q;
    }
    return (int*)0;
}

// purpose:  print a given queue 'list'.  'qname' is a string/title of
//           the queue.  field is used to print a special "offset" of
//           the queue elements.
void os_printQueue(int* list, int* qname, int field) {
    int* q;
    int* tail;

    q = os_getHead(list);
    tail = os_getTail(list);

    if ((int)q == 0)
        return;

    print(qname);
    while (q != tail) {
        print(itoa(os_getProcessID(q), string_buffer, 10, 0));
        if (field > 1) {
            print((int*)"[");
            print(itoa(*(q + field), string_buffer, 10, 0));
            print((int*)"]");
        }
        putchar(',');
        q = (int*)*q;
    }
    print(itoa(os_getProcessID(q), string_buffer, 10, 0));

    if (field > 1) {
        print((int*)"[");
        print(itoa(*(q + field), string_buffer, 10, 0));
        print((int*)"]");
    }

    println();
}


// -----------------------------------------------------------------
// --------------------- PROCESS CONTROL BLOCK ---------------------
// -----------------------------------------------------------------

int* os_createProcess() {
    int *new;

    new = (int*)kmalloc(12*4);

    pid = pid + 1;   // use 0 for the operating system

    *(new + 0) = 0;                  // next pointer
    *(new + 1) = 0;                  // prev pointer
    *(new + 2) = pid;                // PID
    *(new + 3) = 0;                  // PC
    *(new + 4) = (int)kmalloc(34*4); // registers
    *(new + 5) = 0;                  // page fault
    *(new + 6) = 0;                  // is child of init
    *(new + 7) = 0;                  // exit code
    *(new + 8) = 0;                  // has not terminated yet
    *(new + 9) = 0;                  // waits for
    *(new + 10) = (int)kmalloc(pageSize);
    *(new + 11) = (int)kmalloc(pageSize); // assume all entries are 0 -> invalidPage

    return new;
}

int os_getProcessID(int* process) {
    return *(process + 2);
}

int os_getProcessPc(int* process) {
    return *(process + 3);
}

int* os_getProcessRegisters(int* process) {
    return (int*)*(process + 4);
}

int os_getProcessPageFault(int* process) {
    return *(process + 5);
}

int os_addProcessPageFault(int* process) {
    *(process + 5) = *(process + 5) + 1;
    return *(process + 5);
}

int* os_getProcessParent(int* process){
    return (int*)*(process + 6);
}

int os_getProcessExitCode(int* process) {
    return *(process + 7);
}

int os_processTerminated(int* process){
    return *(process + 8);
}

int os_waitsForPID(int* process) {
    return *(process + 9);
}

int* os_getProcessPageTable(int* process) {
    return (int*)*(process + 10);
}

int* os_getProcessPageTableFlags(int* process) {
    return (int*)*(process+ 11);
}


// -----------------------------------------------------------------
// --------------------------- SCHEDULING --------------------------
// -----------------------------------------------------------------

void os_contextSwitch() {
    int *process;
    int *pc_ptr;
    int *userRegs;
    int *pt;
    int *pt_flags;

    os_fifo();
    process = os_getHead(readyQ);

    if (process == (int*)0)
        halt(0);     //FINITO!


    pc_ptr = process+3;
    userRegs = os_getProcessRegisters(process);
    pt = os_getProcessPageTable(process);
    pt_flags = os_getProcessPageTableFlags(process);

    if(0) {
        print("switch to process ");
        print((int*)itoa(os_getProcessID(process),string_buffer,10,0));
        println();
    }
    switch_ctx(pc_ptr, userRegs, pt, pt_flags);

}


void os_fifo() {
    int *headp;
    headp = os_getHead(readyQ);
    os_removeQueue(readyQ, headp);
    os_appendQueue(readyQ, headp);
}


// -----------------------------------------------------------------
// --------------------- FORK HELPER FUNCTIONS ---------------------
// -----------------------------------------------------------------

// purpose:  copy registers from one process to another
void os_copyRegisters(int* fromProcess, int* toProcess) {
    int *from_process_regs;
    int *to_process_regs;
    int i;

    i = 0;
    from_process_regs = os_getProcessRegisters(fromProcess);
    to_process_regs = os_getProcessRegisters(toProcess);

    while (i < 32) {
        *(to_process_regs + i) = *(from_process_regs + i);
        i = i + 1;
    }
}

// purpose: copy page table from one process to another
void os_forkPageTable(int *process, int *new_process) {
    int *from_pTable;
    int *from_pTableFlags;
    int *to_pTable;
    int *to_pTableFlags;
    int rpn;
    int i;

    from_pTable = os_getProcessPageTable(process);
    from_pTableFlags = os_getProcessPageTableFlags(process);
    to_pTable = os_getProcessPageTable(new_process);
    to_pTableFlags = os_getProcessPageTableFlags(new_process);

    i = 0;
    while (i < pageSize/4) {
        if (*(from_pTableFlags + i) != invalidPage) {

            *(from_pTableFlags + i)= readOnlyPage;
            *(to_pTableFlags + i)= readOnlyPage;

            rpn = *(from_pTable + i);
            *(to_pTable + i) = rpn;
            os_incPageFrameRefs(rpn);
        }
        i = i + 1;
    }
}

// purpose: increase the number of references to a physical page rpn
int os_incPageFrameRefs(int rpn) {

    *(pageFrameRefs + rpn) = *(pageFrameRefs + rpn) + 1;
    return *(pageFrameRefs + rpn);
}

// purpose: decrease the number of references to a physical page rpn
int os_decPageFrameRefs(int rpn) {
    *(pageFrameRefs + rpn) = *(pageFrameRefs + rpn) - 1;
    return *(pageFrameRefs + rpn);
}


// -----------------------------------------------------------------
// --------------------------- DEBUGGING ---------------------------
// -----------------------------------------------------------------

void printPageTable(int *process) {
    int *t;
    int *f;
    int i;
    int flag;

    t = os_getProcessPageTable(process);
    f = os_getProcessPageTableFlags(process);
    i = 0;

    print((int*)"page table of process ");
    print((int*)itoa(os_getProcessID(process),string_buffer,10,0));
    println();

    while (i < 1024) {
        flag = *(f+i);
        if (flag != 0) {
            print(itoa(i, string_buffer,10,0));
            print((int*) ": ");
            print(itoa(*(t+i),string_buffer,10,0));
            print((int*) ", flag ");
            print(itoa(flag,string_buffer,10,0));
            println();

        }
        i = i + 1;
    }
    print((int*)"-------------------------");
    println();
}

void printMemory() {
    int i;
    i = 0;
    while (i < (int)userSpace) {
            print(itoa(i, string_buffer, 10, 0));
            print((int*)" : ");
            print(itoa(*((int*)i), string_buffer, 16, 0));
            println();
        i = i + 4;
    }
}

void printRegisters(int* reg) {
    int a;
    a= 0;
    while (a < 32) {
        print("reg ");
        print((int*)itoa(a, string_buffer, 10, 0));
        print(": ");
        print((int*)itoa(*(reg+a), string_buffer, 10, 0));
        println();
        a = a + 1;
    }
}


// -----------------------------------------------------------------
// ----------------------- LIBRARY FUNCTIONS -----------------------
// -----------------------------------------------------------------

int twoToThePowerOf(int p) {
    // assert: 0 <= p < 31
    return *(power_of_two_table + p);
}

int leftShift(int n, int b) {
    // assert: b >= 0;

    if (b > 30)
        return 0;
    else
        return n * twoToThePowerOf(b);
}

int rightShift(int n, int b) {
    // assert: b >= 0

    if (b > 30)
        return 0;
    else if (n >= 0)
        return n / twoToThePowerOf(b);
    else
        // works even if n == INT_MIN
        return ((n + 1) + INT_MAX) / twoToThePowerOf(b) +
            (INT_MAX / twoToThePowerOf(b) + 1);
}

int loadCharacter(int *s, int i) {
    // assert: i >= 0
    int a;

    a = i / 4;

    return rightShift(leftShift(*(s + a), 24 - (i % 4) * 8), 24);
}

int* storeCharacter(int *s, int i, int c) {
    // assert: i >= 0, all characters are 7-bit
    int a;

    a = i / 4;

    *(s + a) = (*(s + a) - leftShift(loadCharacter(s, i), (i % 4) * 8)) + leftShift(c, (i % 4) * 8);

    return s;
}

int stringLength(int *s) {
    int i;

    i = 0;

    while (loadCharacter(s, i) != 0)
        i = i + 1;

    return i;
}

void stringReverse(int *s) {
    int i;
    int j;
    int tmp;

    i = 0;
    j = stringLength(s) - 1;

    while (i < j) {
        tmp = loadCharacter(s, i);

        storeCharacter(s, i, loadCharacter(s, j));
        storeCharacter(s, j, tmp);

        i = i + 1;
        j = j - 1;
    }
}

int stringCompare(int *s, int *t) {
    int i;

    i = 0;

    while (1)
        if (loadCharacter(s, i) == 0)
            if (loadCharacter(t, i) == 0)
                return 1;
            else
                return 0;
        else if (loadCharacter(s, i) == loadCharacter(t, i))
            i = i + 1;
        else
            return 0;
}

int atoi(int *s) {
    int i;
    int n;
    int c;

    i = 0;
    n = 0;
    c = loadCharacter(s, i);

    while (c != 0) {
        c = c - '0';
        if (c < 0)
            return -1;
        else if (c > 9)
            return -1;

        n = n * 10 + c;
        i = i + 1;
        c = loadCharacter(s, i);

        if (n < 0) {
            if (n != INT_MIN)
                return -1;
            else if (c != 0)
                return -1;
        }
    }
    return n;
}

int* itoa(int n, int *s, int b, int a) {
    // assert: b in {2,4,8,10,16}
    int i;
    int sign;

    i = 0;
    sign = 0;

    if (n == 0) {
        storeCharacter(s, 0, '0');
        i = 1;
    } else if (n < 0) {
        sign = 1;

        if (b == 10) {
            if (n == INT_MIN) {
                // rightmost decimal digit of 32-bit INT_MIN
                storeCharacter(s, 0, '8');
                n = -(n / 10);
                i = i + 1;
            } else
                n = -n;
        } else {
            if (n == INT_MIN) {
                // rightmost non-decimal digit of INT_MIN
                storeCharacter(s, 0, '0');
                n = (rightShift(INT_MIN, 1) / b) * 2;
                i = i + 1;
            } else
                n = rightShift(leftShift(n, 1), 1);
        }
    }

    while (n != 0) {
        if (n % b > 9)
            storeCharacter(s, i, n % b - 10 + 'A');
        else
            storeCharacter(s, i, n % b + '0');

        n = n / b;
        i = i + 1;

        if (i == 1) {
            if (sign) {
                if (b != 10)
                    n = n + (rightShift(INT_MIN, 1) / b) * 2;
            }
        }
    }

    if (b != 10) {
        while (i < a) {
            storeCharacter(s, i, '0'); // align with zeros
            i = i + 1;
        }

        if (b == 8) {
            storeCharacter(s, i, '0');
            storeCharacter(s, i + 1, '0');

            i = i + 2;
        } else if (b == 16) {
            storeCharacter(s, i, 'x');
            storeCharacter(s, i + 1, '0');
            i = i + 2;
        }
    } else if (sign) {
        storeCharacter(s, i, '-');
        i = i + 1;
    }

    storeCharacter(s, i, 0); // null terminated string
    stringReverse(s);
    return s;
}

void print(int *s) {
    int i;
    i = 0;

    while (loadCharacter(s, i) != 0) {
        putchar(loadCharacter(s, i));
        i = i + 1;
    }
}

void println() {
    putchar(CHAR_LF);
}

void printCharacter(int character) {
    putchar(CHAR_SINGLEQUOTE);

    if (character == CHAR_EOF)
        print((int*) "end of file");
    else if (character == CHAR_TAB)
        print((int*) "tabulator");
    else if (character == CHAR_LF)
        print((int*) "line feed");
    else if (character == CHAR_CR)
        print((int*) "carriage return");
    else
        putchar(character);

    putchar(CHAR_SINGLEQUOTE);
}

void printString(int *s) {
    putchar(CHAR_DOUBLEQUOTE);
    print(s);
    putchar(CHAR_DOUBLEQUOTE);
}

void initLibrary() {
    int i;

    power_of_two_table = kmalloc(31*4);
    *power_of_two_table = 1; // 2^0
    i = 1;

    while (i < 31) {
        *(power_of_two_table + i) = *(power_of_two_table + (i - 1)) * 2;
        i = i + 1;
    }

    // computing INT_MAX and INT_MIN without overflows
    INT_MAX = (twoToThePowerOf(30) - 1) * 2 + 1;
    INT_MIN = -INT_MAX - 1;

    // accommodate 32-bit numbers for itoa
    string_buffer = kmalloc(33*4);

}
