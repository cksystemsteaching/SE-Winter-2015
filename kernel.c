//Kernel.c
int *args;
int debug = 0; //Set from microkernel
int pMemSize = 0; //Set from microkernel
int* readyQ;

int CMD_BOOT = 1;
int CMD_CREATECONTEXT = 2;
int CMD_SWITCHCONTEXT = 3;
int CMD_DELETECONTEXT = 4;
int CMD_MAPPAGEINCCONTEXT = 5;
int CMD_FLUSHPAGEINCCONTEXT = 6;
int CMD_HALT = 7;
void printBoot();


int main(){
    args = amalloc(4*4,2); //IPC Microkernel <-> kernel process
    if(*args == CMD_BOOT){ //BOOT
       debug = *(args+3);
       pMemSize = *(args+2);
       printBoot(); 
    }else if(*args == CMD_MAPPAGEINCCONTEXT){
       amalloc(1024*4,1); //Aligned Malloc with 4KB = 1Page

    }else if(*args == CMD_FLUSHPAGEINCCONTEXT){
    
    }else if(*args == CMD_SWITCHCONTEXT){
    
    }else if(*args == CMD_DELETECONTEXT){
    
    }else if(*args == CMD_CREATECONTEXT){
    
    }else if(*args == CMD_HALT){
        exit(0);
    }
}

void printBoot(){
    int i;
    i = 0;
    putchar(10);
    putchar(10);
    while(i < 100){
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
    putchar('O');
    putchar('K');
    putchar('E');
    putchar('R');
    putchar('N');
    putchar('E');
    putchar('L');
    putchar(10);
    while(i < 100){
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
    if(debug == 1){
        putchar('T');   
        putchar('r');   
        putchar('u');   
        putchar('e');   
    }else{
        putchar('F');
        putchar('a');
        putchar('l');
        putchar('s');
        putchar('e');
    }
    putchar(10);
}
