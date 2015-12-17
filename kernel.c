//Kernel.c
int* args;
int debug = 0;    //Set from microkernel
int pMemSize = 0; //Set from microkernel
int* readyQ = (int*)0;
int pId = 1;
int VMEMSIZE = 4194304;
int CMD_BOOT = 1;
int CMD_CREATECONTEXT = 2;
int CMD_SWITCHCONTEXT = 3;
int CMD_DELETECONTEXT = 4;
int CMD_MAPPAGEINCCONTEXT = 5;
int CMD_FLUSHPAGEINCCONTEXT = 6;
int CMD_HALT = 7;
void printBoot();
int* create_Context();
void delete_Context();

int main()
{
    int* node;
    args = amalloc(4 * 4, 2); //IPC Microkernel <-> kernel process
    if (*args == CMD_BOOT) {  //BOOT
        debug = *(args + 3);
        pMemSize = *(args + 2);
        printBoot();
        node = create_Context();
        loadBinary(*(node + 1), *(node + 2), *(node + 3), *(node + 4));
    }
    else if (*args == CMD_MAPPAGEINCCONTEXT) {
        node = (int*)*(readyQ +2);
        node = node + *(args + 1); //vpn
        *node = amalloc(1024 * 4, 1); //Aligned Malloc with 4KB = 1Page
        mapPageInContext();
    }
    else if (*args == CMD_FLUSHPAGEINCCONTEXT) {
    }
    else if (*args == CMD_SWITCHCONTEXT) {
        if((int)readyQ == 0){
            exit(1);
        }
        *(readyQ +1) = *(args + 1); //Save pc
        *(readyQ +2) = *(args + 2); //Save pT
        *(readyQ +3) = *(args + 3); //Save registers
        readyQ = (int*)*readyQ;
        switchContext(*(readyQ + 1),*(readyQ + 2),*(readyQ + 3), *(readyQ + 4));
        
    }
    else if (*args == CMD_DELETECONTEXT) {
        delete_Context();
        if((int)readyQ == 0){
            deleteContext(0,0,0,0);
        }else{
            deleteContext(*(readyQ + 1),*(readyQ + 2),*(readyQ + 3), *(readyQ + 4));
        }
    }
    else if (*args == CMD_CREATECONTEXT) {
        node = create_Context();
        createContext(*(node + 1), *(node + 2), *(node + 3), *(node + 4));
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
    //*(ptr + 1023) = (int)amalloc(1024*4,1);
    return node;
}

void delete_Context(){
    int *ptr;
    int *newStart;

    ptr = readyQ;
    //Is last entry?
    if(*ptr == (int)ptr){
        readyQ = (int*)0;
    }else{
        newStart = (int*)*ptr;
        while(*ptr != (int)readyQ){
            ptr = (int*)*ptr;
        }
        *ptr = (int)newStart;
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
    putchar('U');
    putchar('S');
    putchar('E');
    putchar('G');
    putchar(' ');
    putchar('M');
    putchar('I');
    putchar('C');
    putchar('R');
    putchar('O');
    putchar('K');
    putchar('E');
    putchar('R');
    putchar('N');
    putchar('E');
    putchar('L');
    putchar(10);
    while (i < 100) {
        putchar('=');
        i = i + 1;
    }
    putchar(10);
    putchar('D');
    putchar('e');
    putchar('b');
    putchar('u');
    putchar('g');
    putchar(':');
    putchar(' ');
    if (debug == 1) {
        putchar('T');
        putchar('r');
        putchar('u');
        putchar('e');
    }
    else {
        putchar('F');
        putchar('a');
        putchar('l');
        putchar('s');
        putchar('e');
    }
    putchar(10);
}
