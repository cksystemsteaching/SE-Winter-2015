int whichServer;                // timer, pager, or syscall
int v0;
int a0;
int a1;
int a2;
int a3;

int TIMER = 5;
int PAGER = 6;
int SYSCALL = 7;


int SYSCALL_EXIT    = 4001;
int SYSCALL_FORK    = 4002;
int SYSCALL_WAIT    = 4007;
int SYSCALL_GETPID  = 4020;
int SYSCALL_YIELD   = 4162;
int SYSCALL_LOCK    = 6000;
int SYSCALL_UNLOCK  = 6001;

int bumpPtr;

int *readyQ;

int main(int kernelMemory, int kernelGP) {
    init(kernelGP+4);
    server_timer();

    while (1) {
        if (whichServer == TIMER) {
            server_timer();
        } else if (whichServer == PAGER) {
            server_pager();
        } else if (whichServer == SYSCALL) {
            server_syscall();
        } else {
            putchar('E');
        }
    }
}


int init(int gp) {
    int *initProcess;
    bumpPtr = gp;
    readyQ = os_createQueue();

    initProcess = kmalloc(3*4);     // manual generate first process
    *(initProcess + 0) = 0;
    *(initProcess + 1) = 0;
    *(initProcess + 2) = 2;           // start with PID 2.
    os_appendQueue(readyQ, initProcess);
}


// purpose:  create and initialize a queue, return pointer to it.
//           a queue consists of a pointer to a head and a tail.
int* os_createQueue() {
    int *q;
    q = (int*)malloc(2*4);
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





int* createProcess() {
    int *new;
    new = kmalloc(3*4);
    *(new + 1) = create_ctx();       // context id / pid

    os_appendQueue(readyQ, new);
                   //server_timer();

    return new;
}



int* kmalloc(int size) {
    int oldBumpPtr;

    oldBumpPtr = bumpPtr;
    bumpPtr = bumpPtr + size;

    return oldBumpPtr;
}


void os_fifo() {
    int *headp;
    headp = os_getHead(readyQ);
    os_removeQueue(readyQ, headp);
    os_appendQueue(readyQ, headp);
}



int server_timer() {
    int *new;
    os_fifo();
    new = os_getHead(readyQ);
    switch_ctx(*(new + 2));
}

int server_pager() {
  // page fault in emulator: 
  // trap in to kernel/start kernel process, allocate 
  // page frame and then, map the page into the page table 
  // from the client process using 
  //       ....  map_page_in_context
}

int syscall_exit() {

    // remove process from list,
    // free pages with flush_page_in_context and then, 
    // delete the context in the emulator.  

    // flush_page_in_ctx( .. )

    delete_ctx(*(os_getHead(readyQ)+2));
}

int syscall_fork() {
    // create_ctx ( .. )
    // allocate pages with: 
    // map_pages_in_ctx( ..)
}

int server_syscall() {
    // on exit: flush_page_in_context, delete ctx
    // fork   : create_ctx
    //
    if (v0 == SYSCALL_EXIT) {
        syscall_exit();
    } else if (v0 == SYSCALL_FORK) {

    } else if (v0 == SYSCALL_GETPID) {

    } else if (v0 == SYSCALL_YIELD) {

    } else if (v0 == SYSCALL_LOCK) {

    } else if (v0 == SYSCALL_UNLOCK) {

    } else {
        // unkown syscall
    }
}

