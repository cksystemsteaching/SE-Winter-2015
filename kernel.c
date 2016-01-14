
// a context has following structure:
//
// 0 +-----+
//   |prev | pointer to previous element in list
// 1 +-----+  
//   |next | pointer to next element in list
// 2 +-----+  
//   | uid | unique PID
// 3 +-----+  
//   | pc  | program counter
// 4 +-----+  
//   |reg  | pointer to registers
// 5 +-----+  
//   | PT  | pointer to page table
// 6 +-----+ 

//------------- methods -------------------------
int* createContext();
void createFirstContext();
void switchContext();
void deleteContext();
void mapPageInContext();
void flushPageInContext();

int* initList();
void appendContext(int *context);
int* removeFirstContext();
int* createEmptyPageTable();
int* createEmptyRegisters();

//	--- GETTER ---
int* getPrev(int *context);
int* getNext(int *context);
int  getUID(int *context);
int  getPC(int *context);
int* getRegisters(int *context);
int* getPageTable(int *context);

//	--- SETTER ---
void setPrev(int *context, int *prev);
void setNext(int *context, int *next);
void setUID(int *context, int uid);
void setPC(int *context, int pc);
void setRegisters(int *context, int *regs);
void setPageTable(int *context, int *pt);

//------------- constants ------------------------

int ANY2 = 36;
int ANY = 34;
int HYPERCALL_CREATECONTEXT		= 7001;
int HYPERCALL_SWITCHCONTEXT		= 7002;
int HYPERCALL_DELETECONTEXT		= 7003;
int HYPERCALL_MAPPAGEINCONTEXT	= 7004;
int HYPERCALL_FLUSHPAGEINCONTEXT= 7005;
int HYPERCALL_LOADBINARY		= 7006;

int VMEMORYSIZE = 4194304;	// Byte, 4MB
int PAGESIZE = 4096;		// Byte, 4KB
int BOOT = 1;
//------------- variables ------------------------
int *readyQueue = (int*)0;
int *currContext = (int*)0;
int nextValidUID = 1;
int *cmd;

int main(){
	

//	switchContext();
	if(BOOT){
		cmd = allocCMD();
		BOOT = 1;
		putchar(10);
		putchar('k');
		putchar('e');
		putchar('r');
		putchar('n');
		putchar('e');
		putchar('l');
		putchar(10);
	
	}
	//*cmd = *cmd -1
	if(*cmd == ANY){
		putchar(10);
		putchar('a');
		putchar('n');
		putchar('y');
		putchar(10);
	}
	else if(*cmd == HYPERCALL_CREATECONTEXT){
		putchar(10);
		putchar('c');
		putchar('r');
		putchar('e');
		putchar('a');
		putchar('t');
		putchar('e');
		putchar(10);
	} else if(*cmd == HYPERCALL_SWITCHCONTEXT){
		putchar(10);
		putchar('s');
		putchar('w');
		putchar('i');
		putchar('t');
		putchar('c');
		putchar('h');
		putchar(10);

//		currContext = createContext();
	} else if(*cmd == HYPERCALL_DELETECONTEXT){
		putchar(10);
		putchar('d');
		putchar('e');
		putchar('l');
		putchar('e');
		putchar('t');
		putchar('e');
		putchar(10);
	} else if(*cmd == HYPERCALL_MAPPAGEINCONTEXT){
		putchar(10);
		putchar('m');
		putchar('a');
		putchar('p');
		putchar(10);
	} else if(*cmd == HYPERCALL_FLUSHPAGEINCONTEXT){
		putchar(10);
		putchar('f');
		putchar('l');
		putchar('u');
		putchar('s');
		putchar('h');
		putchar(10);
	}
	putchar(10);
//		putchar('e');
//		putchar('n');
//		putchar('d');
//	putchar(10);
	
	exit(0);
}

int* createContext(){
	int *context;
	context = malloc(6*4);
	setPrev(context, (int*)0);
	setNext(context, (int*)0);
	setUID(context, nextValidUID);
	setPC(context, 0);
	setRegisters(context, createEmptyRegisters());
	setPageTable(context, createEmptyPageTable());
	
	nextValidUID = nextValidUID + 1;

	hc_createContext(getUID(context), getPC(context), getRegisters(context), getPageTable(context));
    appendContext(readyQueue);  //I think we should append any new context????
	return context;
}


void switchContext(){
	if((int)currContext == 0){
		appendContext(currContext);
	}
	currContext = removeFirstContext();
}

void deleteContext(){
	currContext = (int*)0;
	// we need to delete it from the contextList????????????
}
void mapPageInContext(){//{int *context, int *page){
	int *pageTable;
	pageTable = getPageTable(currContext);
}

void flushPageInContext(int *context, int *page){
}


int* createEmptyRegisters(){
	return malloc(32*4);
}

// --- PageTable ---
int* createEmptyPageTable(){
	int *pt;
	int ptMaxEntries;
	int ptSize;
	int i;
	i=0;
	
	ptMaxEntries = VMEMORYSIZE / PAGESIZE;
	ptSize = ptMaxEntries * 4;
	pt = malloc(ptSize);

	while(i < ptSize){
		*(pt+i) = 0;
		i = i + 1;
	}
	
	return pt;
}

//	--- GETTER ---
int* getPrev(int *context){
	return (int*)*context;
}
int* getNext(int *context){
	return (int*)*(context+1);
}
int getUID(int *context){
	return *(context+2);
}
int  getPC(int *context){
	return *(context+3);
}
int* getRegisters(int *context){
	return (int*)*(context+4);
}
int* getPageTable(int *context){
	return (int*)*(context+5);
}

//	--- SETTER ---
void setPrev(int *context, int *prev){
	*context = (int)prev;
}
void setNext(int *context, int *next){
	*(context+1) = (int)next;
}
void setUID(int *context, int uid){
	*(context+2) = uid;
}
void setPC(int *context, int pc){
	*(context+3) = pc;
}
void setRegisters(int *context, int *regs){
	*(context+4) = (int)regs;
}
void setPageTable(int *context, int *pt){
	*(context+5) = (int)pt;
}

//  --- LIST ---
int* initList(){
	int *list;
	list = malloc(2*4);
	*list = 0;
	*(list+1) = 0;
	return list;
}

void appendContext(int *context){
	int *tail;
	tail = (int*)*(readyQueue+1);
	if(isListEmpty()){
		*readyQueue = (int)context;
	} else {
		*context = *(readyQueue+1);
		*(readyQueue+1) = (int)context;
	}
	*(readyQueue+1) = (int)context;
	*(context+1) = 0;
}
int* removeFirstContext(){
	int *head;
	int *tail;
	int *remove;
	head = (int*)*readyQueue;
	tail = (int*)*(readyQueue+1);
	
	if((int)head == 0){
		remove = (int*)0;
	} else if((int)getNext(head) == 0){
		remove = head;
		head = (int*)0;
		tail = (int*)0;
	} else {
		remove = head;
		head = getNext(head);
	}
	return remove;
}
int isListEmpty(){
	if(*readyQueue == 0)
		return 1;
	return 0;
}

