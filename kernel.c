//Kernel.c
int* args;
int debug = 0;    //Set from microkernel
int pMemSize = 0; //Set from microkernel
int bootPrep = 1;
int* node;
int* readyQ = (int*)0;
int* pFree = (int*)0;
int pId = 1;
int VMEMSIZE = 4194304;
int CMD_BOOT = 1;
int CMD_CREATECONTEXT = 2;
int CMD_SWITCHCONTEXT = 3;
int CMD_DELETECONTEXT = 4;
int CMD_MAPPAGEINCCONTEXT = 5;
int CMD_FLUSHPAGEINCCONTEXT = 6;
int CMD_HALT = 7;
int CMD_TFORK = 8;

void printBoot();
int* create_Context();
void delete_Context();
int* getPFree();
void addPFree(int* addr);

int* dummyP1;
int* dummyP2;
int i;
int main()
{
    if (bootPrep == 1) {
        args = amalloc(4 * 4, 2); //IPC Microkernel <-> kernel process
        bootPrep = 0;
    }
    if (*args == CMD_BOOT) { //BOOT
        debug = *(args + 3);
        pMemSize = *(args + 2);
        printBoot();
        node = create_Context();
        mc_loadBinary(*(node + 1), *(node + 2), *(node + 3), *(node + 4));
    }
    else if (*args == CMD_MAPPAGEINCCONTEXT) {
        node = (int*)*(readyQ + 2);
        node = node + *(args + 1); //vpn
        if ((int)pFree == 0) {
            *node = (int)amalloc(1024 * 4, 1); //Aligned Malloc with 4KB = 1Page
        }
        else {
            *node = (int)getPFree();
        }
        mapPageInContext();
    }
    else if (*args == CMD_FLUSHPAGEINCCONTEXT) {
    }
    else if (*args == CMD_SWITCHCONTEXT) {
        if ((int)readyQ == 0) {
            exit(1);
        }
        *(readyQ + 1) = *(args + 1); //Save pc
        *(readyQ + 2) = *(args + 2); //Save pT
        *(readyQ + 3) = *(args + 3); //Save registers
        readyQ = (int*)*readyQ;
        switchContext(*(readyQ + 1), *(readyQ + 2), *(readyQ + 3), *(readyQ + 4));
    }
    else if (*args == CMD_DELETECONTEXT) {
        delete_Context();
        if ((int)readyQ == 0) {
            deleteContext(0, 0, 0, 0);
        }
        else {
            deleteContext(*(readyQ + 1), *(readyQ + 2), *(readyQ + 3), *(readyQ + 4));
        }
    }
    else if (*args == CMD_CREATECONTEXT) {
        node = create_Context();
        createContext(*(node + 1), *(node + 2), *(node + 3), *(node + 4));
    }else if(*args == CMD_TFORK){
       *(readyQ + 1) = *(args + 1); //Save pc
       *(readyQ + 2) = *(args + 2); //Save pT
       *(readyQ + 3) = *(args + 3); //Save registers
       node = create_Context();
       //Copy pageTable unitl we read a zero, or we are on the last 8KB
       dummyP1 = (int*)*(node+2);
       dummyP2 =(int*)*(readyQ + 2);
       i = 0;
       while(*(dummyP2+i) != 0){
            *(dummyP1 + i) = *(dummyP2 + i);
            i = i + 1;
       }
       i = 0;
       dummyP1 = (int*)*(node + 3);
       dummyP2 = (int*)*(readyQ + 3);
       while(i < 32){
            *(dummyP1 + i) = *(dummyP2 + i);
            i = i + 1;
       }
       *(dummyP1 + 2) = 0; //Child has internal PID 0 after fork
       *(dummyP2 + 2) = *(node + 4); //Parent has in REG_V0 the pid of the child
       *(node + 1) = *(readyQ + 1);
       createContext(*(readyQ + 1), *(readyQ + 2), *(readyQ + 3), *(readyQ + 4)); //Restore Parent
    }
    else if (*args == CMD_HALT) {
        exit(0);
    }
}
//PList
//| Next |
//| PC   |
//| pT   |
//| reg  |
//| pId  |
int* create_Context()
{
    int* node;
    int* ptr;
    ptr = readyQ;
    node = malloc(5 * 4);
    if ((int)readyQ == 0) {
        readyQ = node;
        *node = (int)readyQ;
    }
    else {
        while (*ptr != (int)readyQ) {
            ptr = (int*)*ptr;
        }
        *ptr = (int)node;
        *node = (int)readyQ;
    }
    *(node + 1) = 0;
    *(node + 2) = (int)malloc(VMEMSIZE / 4);
    *(node + 3) = (int)malloc(32 * 4);
    *(node + 4) = pId;
    pId = pId + 1;
    ptr = (int*)*(node + 3);
    *(ptr + 29) = VMEMSIZE - 4;
    //We Hardcode for now:
    //We set the first 8kb vmem to
    //load the binary later we will do this
    //in the OS
    ptr = (int*)*(node + 2);
    *ptr = (int)amalloc(1024 * 4, 1);
    *(ptr + 1) = (int)amalloc(1024 * 4, 1);
    return node;
}

void delete_Context()
{
    int* ptr;
    int* newStart;
    int i;
    int* pTable;
    i = 0;
    ptr = readyQ;
    //pTable = (int*)*(ptr + 2);
    //Recycle pages
    //while (i < 1024) {
    //    if (*(pTable + i) != 0) {
    //        addPFree(pTable + i);
    //    }
    //    i = i + 1;
    //}
    //Is last entry?
    if (*ptr == (int)ptr) {
        readyQ = (int*)0;
    }
    else {
        newStart = (int*)*ptr;
        while (*ptr != (int)readyQ) {
            ptr = (int*)*ptr;
        }
        *ptr = (int)newStart;
        readyQ = newStart;
    }
}

void printBoot()
{
    int i;
    i = 0;
    putchar(10);
    putchar(10);
    while (i < 100) {
        putchar('=');
        i = i + 1;
    }
    i = 0;
    putchar(10);
    l_print((int*)"USEG MICROKERNEL");
    while (i < 100) {
        putchar('=');
        i = i + 1;
    }
    putchar(10);
    if (debug == 1) {
        l_print((int*)"Debug: True");
    }
    else {
        l_print((int*)"Debug: False");
    }
    putchar(10);
}

//Freelist
//|Next|
//|addr|

int* getPFree()
{
    int* addr;
    if ((int)pFree != 0) {
        addr = (int*)*(pFree + 1);
        pFree = (int*)*pFree;
        return addr;
    }
    return (int*)0;
}

void addPFree(int* addr)
{
    int* node;
    int* ptr;
    if ((int)pFree == 0) {
        pFree = malloc(2 * 4);
        *pFree = 0;
        *(pFree + 1) = (int)addr;
    }
    else {
        node = malloc(2 * 4);
        *(node + 1) = (int)addr;
        *node = 0;
        ptr = pFree;
        while (*ptr != 0) {
            ptr = (int*)*ptr;
        }
        *ptr = (int)node;
    }
}
