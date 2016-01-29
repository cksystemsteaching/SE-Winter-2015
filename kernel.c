//--- variables ---

int *currProcess;
int *currPageTable;
int *processQueue;
int pid;
int *ipc;
int *binaryName;
int *pfreeList;


// --- constants ---
int PAGESIZE = 4096;		// Byte, 4KB
int PAGEFRAMESIZE = 4096;	// Byte, 4KB
int VMEMORYSIZE = 4194304;	// Byte, 4MB

int CREATECONTEXT = 1;
int SWITCHCONTEXT = 2;
int DELETECONTEXT = 3;
int MAPPAGEINCONTEXT = 4;
int FLUSHPAGEINCONTEXT = 5;
int EXITKERNEL = 6;

// --- methods ---
int* createProcess();
int* createPageTable();
void loadBinary();
int* initFreeList(int *memoryStart, int memorySize);
int  palloc();
void mapPageInContext(int pid, int ptOffset);
int* initList();
int* findProcessByPid(int *queue, int pid);
int* removeFirst(int *queue);
void deleteProcess(int *queue, int pid);
int* removeFromList(int *queue, int pid);
void appendProcess(int *queue, int *process);
int* pollHead(int *queue);
int* pollTail(int *queue);
void printProcessQueue(int *queue);
void printNumber(int number);
void setPrevProcess(int *process, int *prev);
void setNextProcess(int *process, int *next);
void setPid(int *process, int pid);
void setPageTable(int *process, int *pt);

void setPrevProcess(int *process, int *prev);
void setNextProcess(int *process, int *next);
void setPid(int *process, int pid);

int* getPrevProcess(int *process);
int* getNextProcess(int *process);
int  getPid(int *process);
int* getPageTable(int *process);

int main(){
		

	ipc = allocIpc(4*4);
	pfreeList = initFreeList((int*)*(ipc+1), *(ipc+2));
	processQueue = initList();
	
	while(1){
		if(*ipc == CREATECONTEXT){
			putchar('c');
			putchar('r');
			putchar('e');
			putchar('a');
			putchar('t');
			putchar('e');
			putchar(10);

			currProcess = createProcess();
			hcLoadBinary(getPid(currProcess));
		
		} else if(*ipc == SWITCHCONTEXT){
			
			if((int)currProcess != 0)
				appendProcess(processQueue, currProcess);
			currProcess = (int*)0;
			currProcess = removeFirst(processQueue);
			if((int)currProcess != 0){
				hcSwitchContext(getPid(currProcess));
			} else 
				exit(10);		
		} else if(*ipc == DELETECONTEXT){
			putchar('d');
			putchar('e');
			putchar('l');
			putchar('e');
			putchar('t');
			putchar('e');
			putchar(' ');
			putchar('p');
			putchar('i');
			putchar('d');
			putchar(' ');
			putchar('[');
			putchar(getPid(currProcess)+'0');
			putchar(']');
			putchar(10);
			

			deleteProcess(processQueue, getPid(currProcess));
		} else if(*ipc == MAPPAGEINCONTEXT){
			putchar('m');
			putchar('a');
			putchar('p');
			putchar(10);
			mapPageInContext(*(ipc+1), *(ipc+2));
			
			hcMapPageInContext(*(ipc+1), *(ipc+2));
		
		} else if(*ipc == FLUSHPAGEINCONTEXT){
			putchar('f');
			putchar('l');
			putchar('u');
			putchar('s');
			putchar('h');
			putchar(10);
		
		} else if(*ipc == EXITKERNEL){
			exit(0);
		
			putchar(10);
			putchar('E');
			putchar('X');
			putchar('I');
			putchar('T');
			putchar(10);
			exit(0);
		
		}
	}
}


int* createProcess(){
	int *process;
	int pid;

	process = malloc(4*4);
	pid = hcCreateContext();
	
	setPid(process, pid);
	setPageTable(process, createPageTable());
	return process;
}
int* createPageTable(){
	int ptEntries;
	int ptSize;
	int *pt;
	int i;
	i = 0;
	ptEntries = VMEMORYSIZE/PAGESIZE;
	ptSize = ptEntries *4;
	pt = malloc(ptSize);
	while(i < ptEntries){
		*(pt+i) = 0;
		i = i + 1;
	}
}
void mapPageInContext(int pid, int ptOffset){
	int *process;
	int *pt;
	int pageFrame;
	if((int)currProcess != 0){
		if(getPid(currProcess) == pid)
			process = currProcess;
		else
			process = findProcessByPid(processQueue, pid);
		
	} else
		process = findProcessByPid(processQueue, pid);
	if((int)process != 0){
		pageFrame = palloc();
		pt = getPageTable(process);
		*(pt + ptOffset) = pageFrame;
		
		hcMapPageInContext(pid, ptOffset, (int)pageFrame);
	} else {
	
		exit(-1);
	}
}
void loadBinary(){
	hcLoadBinary(getPid(currProcess));

}
int* initFreeList(int *memoryStart, int memorySize){
	int *list;
	int i;
	int counterPageFrames;
	
	i = 0;
	counterPageFrames = memorySize / PAGEFRAMESIZE;
	list = memoryStart;
	while(i < counterPageFrames){
		if(i+1 == counterPageFrames)
			*(memoryStart+i) = 0;//(int)memoryStart + PAGEFRAMESIZE;
		else
			*(memoryStart+i) = (int)memoryStart + (i+1) * PAGEFRAMESIZE;
		i = i +1;
	}
	return list;
}

int palloc(){
	int pageFrame;

	if(*pfreeList != 0){
		pageFrame = *pfreeList;
		pfreeList = (int*)*pfreeList;
		return pageFrame;
	}
	exit(-1);

}

int* initList(){
	int *list;
	list = malloc(2 * 4);
	*list = 0;
	*(list+1) = 0;
}

int* pollHead(int *queue){
	return (int*)*queue;
}
int* pollTail(int *queue){
	return (int*)*(queue+1);
}
int* findProcessByPid(int *queue, int pid){
	int *curr;
	curr = pollHead(queue);
	while((int)curr != 0){
		if(getPid(curr) == pid)
			return curr;
		curr = getNextProcess(curr);
	}
	return curr; //curr = (int*)0
}
int* removeFirst(int *queue){
	int *first;
	if(*queue == 0)
		return (int*)0;
	first = pollHead(queue);
	if(*queue == (int)pollTail(queue)){
		*queue = 0;
		*(queue + 1) = 0;
	} else {
		*queue = (int)getNextProcess(first);
	}
	setNextProcess(first, (int*)0);
	return first;
}

void deleteProcess(int *queue, int pid){
	int *process;
	process = (int*)0;
	if((int)currProcess != 0){
		if(getPid(currProcess) == pid){
			process = currProcess;
			currProcess = (int*)0;
		} else
			process = removeFromList(queue, pid);
	} else 
		process = removeFromList(queue, pid);
	
	if((int)process != 0)	
		hcDeleteContext(pid);
}
int* removeFromList(int *queue, int pid){
	int *process;
	int *prev;
	int *next;
	
	if(*queue != 0)
		return (int*)0;
	
	process = findProcessByPid(queue, pid);
	if((int)process == 0)
		return (int*)0;
	if(getPid(process) == getPid(pollHead(queue))){		// process is first element
		removeFirst(queue);
	} else if(getPid(process) == getPid(pollTail(queue))){ // process is not first but last element
		prev = getPrevProcess(process);
		setNextProcess(prev, (int*)0);
		*(queue+1) = (int)prev;
	} else {						// process is in middle of queue
		prev = getPrevProcess(process);
		next = getNextProcess(process);
		*(prev+1) = (int)next;
		*next = (int)prev;
	}
	return process;
}
void appendProcess(int *queue, int *process){
	int *tail;
	tail = pollTail(queue);
	if((int)tail == 0){
		*queue = (int)process;
	} else {
		setNextProcess(tail, process);
		setPrevProcess(process, tail);
	}
	setNextProcess(process, (int*)0);
	*(queue+1) = (int)process;
}

void printProcessQueue(int *queue){
	int *curr;
	curr = pollHead(queue);
	putchar(10);
	putchar('L');
	putchar('I');
	putchar('S');
	putchar('T');
	putchar(' ');
	putchar('S');
	putchar('T');
	putchar('A');
	putchar('R');
	putchar('T');
	putchar(10);
	if((int)curr == 0){
		putchar(' ');
		putchar(' ');
		putchar('E');
		putchar('M');
		putchar('P');
		putchar('T');
		putchar('Y');
	
	} else {
		putchar('p');
		putchar('i');
		putchar('d');
		putchar(':');
		putchar(' ');
	}
	while((int)curr != 0){
		putchar(getPid(curr)+'0');
		curr = getNextProcess(curr);
		if((int)curr != 0){
			putchar(',');
			putchar(' ');
		}
	}
	putchar(10);
	putchar('L');
	putchar('I');
	putchar('S');
	putchar('T');
	putchar(' ');
	putchar('E');
	putchar('N');
	putchar('D');
	putchar(10);
	
}

void printNumber(int number){
	int size;
	int subtract;
	int numberCopy;
	int *numbArray;
	int i;
	i = 0;
	subtract = 10;
	size = 1;
	numberCopy = number;
	while(number > 0){
		if(number - subtract < 0 ){
			number = number -subtract;
		} else {
			subtract = subtract * 10;
			size = size +1;
		}
	}
	numbArray = malloc(size*4);
	number = numberCopy;	
	while(i < size ){
		*(numbArray+i) = number%10;
		number = number /10;
		i = i+1;
	}
	putchar(10);
	while(size > 0 ){
		i = *(numbArray+(size-1));
		putchar(i+'0');
		size = size -1;	
	}
	putchar(10);
}

// --- SETTER ---
void setPrevProcess(int *process, int *prev){
	*process = (int)prev;
}
void setNextProcess(int *process, int *next){
	*(process+1) = (int)next;
}
void setPid(int *process, int pid){
	*(process+2) = pid;
}
void setPageTable(int *process, int *pt){
	*(process+3) = (int)pt;
}

// --- GETTER ---
int* getPrevProcess(int *process){
	return (int*)*process;
}
int* getNextProcess(int *process){
	return (int*)*(process+1);
}
int getPid(int *process){
	return *(process+2);
}
int* getPageTable(int *process){
	return (int*)*(process+3);
}

// =======================================================
//A8
void init_Tstack(int *head){
	int * node; // struct node_
	int * ptr; // struct ptr_

    node = create_node();
    ptr = get_node_next(node);
    set_pointer_node(top,node);

}

void push_Tstack(int *head, int value){
	int * newNode;
	int old;
	newNode = create_node();
	set_node_value(newNode, value);
	set_node_next(newNode, 0);
	while(1){
		old = get_node_value(get_pointer_node(top));
		if(cas(top,old,newNode) == 1){
			set_pointer_node(top,newNode);
			set_pointer_index(top,get_pointer_count(top) + 1);	
			return 1;
		}	
		
	}
}

int  pop_Tstack(){
	
	int *first_element;	
    int *old;
    int *next;
    int value;
    
    if (*top == 0)
    	return 0;
    old = (int*)*top;
    value = *(old + 1);
    next = (int*)*old;
    while (1) {
        if (cas(top, old, next) == 1) {
        	
         	first_element = (int*)*top;
         	old = top;
         set_pointer_index(top,get_pointer_count(top) - 1);	
            return 1;
        }
    }
}

int  *create_pointer(){
	int pointer_;
	pointer_ = (int *)malloc(2*4);
	set_pointer_node(pointer_, 0);
	set_pointer_index(pointer_, 0);
	return pointer_;
}

void set_pointer_node(int *pointer, int *ptr){
	*pointer = (int *)ptr;
}
void set_pointer_index(int *pointer, int index){
	*(pointer + 1) = index;
}
int  *get_pointer_node(int *pointer){
	return (int *)pointer;
}
int  get_pointer_count(int *pointer){
	return *(pointer + 1);
}

int  *create_node(){
	int *node;
	int *next;
	node = (int *)malloc(2*4);
	next = create_pointer();
	set_node_value(node, 0);
	set_node_next (node,next);
	return node;
}
void set_node_value(int *node, int value){
	*node = value;
}
void set_node_next(int *node, int *next){
	*(node + 1) = (int)next ;
}
int  get_node_value(int *node){
	return *node;
}
int  *get_node_next(int *node){
	return (int *)*(node + 1);
}

//=============================================




