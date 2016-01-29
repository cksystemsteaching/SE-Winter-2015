//--- variables ---

int *currProcess;
int *currPageTable;
int *processQueue;
int  pid;
int *ipc = (int*)0;
int *pfreeList;
int debug = 0;

// --- constants ---
int PAGESIZE = 4096;		// Byte, 4KB
int PAGEFRAMESIZE = 4096;	// Byte, 4KB
int VMEMORYSIZE = 4194304;	// Byte, 4MB

int CREATECONTEXT = 1;
int SWITCHCONTEXT = 2;
int DELETECONTEXT = 3;
int MAPPAGEINCONTEXT = 4;
int FLUSHPAGEINCONTEXT = 5;
int LOADUSERBINARY = 6;
int EXITKERNEL = 7;

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

int* getPrevProcess(int *process);
int* getNextProcess(int *process);
int  getPid(int *process);
int* getPageTable(int *process);

int main(){
//	if((int)ipc == 0){
		ipc = allocIpc(4*4);
		pfreeList = initFreeList((int*)*(ipc+1), *(ipc+2));
		processQueue = initList();
//	}
	while(1){
//	putchar('a');
//	putchar(' ');
		if(*ipc == CREATECONTEXT){
			if(debug){
				putchar('C');
				putchar('R');
				putchar('E');
				putchar('A');
				putchar('T');
				putchar('E');
				putchar(10);
			}
			currProcess = createProcess();
			//exit(CREATECONTEXT);
		} else if(*ipc == SWITCHCONTEXT){
			if(debug){
				putchar('S');
				putchar('W');
				putchar('I');
				putchar('T');
				putchar('C');
				putchar('H');
				putchar(10);
			}
			
			
			if((int)currProcess != 0)
				appendProcess(processQueue, currProcess);
//			printProcessQueue(processQueue);
			currProcess = (int*)0;
			currProcess = removeFirst(processQueue);
//			printProcessQueue(processQueue);
				//putchar('p');
				//putchar('i');
				//putchar('d');
				//putchar(' ');
				//putchar(getPid(currProcess));
				//putchar(10);
			if((int)currProcess != 0){
				//putchar('p');
				//putchar('i');
				//putchar('d');
				//putchar(' ');
				hcSwitchContext(getPid(currProcess));

			} else 
				*ipc = EXITKERNEL;//exit(SWITCHCONTEXT);		
		} else if(*ipc == DELETECONTEXT){
			if(debug){
				putchar('D');
				putchar('E');
				putchar('L');
				putchar('E');
				putchar('T');
				putchar('E');
				putchar(10);
			}			
			//exit(DELETECONTEXT);
			deleteProcess(processQueue, getPid(currProcess));
		} else if(*ipc == MAPPAGEINCONTEXT){
			if(debug){
				putchar('M');
				putchar('A');
				putchar('P');
				putchar(10);
			}
			mapPageInContext((int*)*(ipc+1), *(ipc+2));
		
		} else if(*ipc == FLUSHPAGEINCONTEXT){
			if(debug){
				putchar('F');
				putchar('L');
				putchar('U');
				putchar('S');
				putchar('H');
				putchar(10);
			}
			exit(FLUSHPAGEINCONTEXT);
		} else if(*ipc == LOADUSERBINARY){
			if(debug){
				putchar('L');
				putchar('O');
				putchar('A');
				putchar('D');
				putchar(10);
			}
			//exit(0);
			hcLoadBinary(*(ipc+1));
		} else if(*ipc == EXITKERNEL){
			if(debug){
				putchar('E');
				putchar('X');
				putchar('I');
				putchar('T');
				putchar(10);
			}		
			exit(0);
		} else {
			putchar('E');
			putchar('R');
			putchar('R');
			putchar('O');
			putchar('R');
			putchar(' ');
			putchar('E');
			putchar('X');
			putchar('I');
			putchar('T');
			putchar(10);
			exit(-1);
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
		
//		putchar(10);
//		putchar(10);
	//	putchar(10);
//		putchar(ptOffset + '0');
		//printNumber(pageFrame);
//		putchar(10);
//		putchar(10);
	//	putchar(10);
		hcMapPageInContext(pid, ptOffset, pageFrame);
	} else {
	
		exit(-1);
	}
}
void loadBinary(){
	//*ipc = (int)binaryName;
	hcLoadBinary(getPid(currProcess));

}
int* initFreeList(int *memoryStart, int memorySize){
	int *list;
	int i;
	int counterPageFrames;
	
	//printNumber((int)memorySize);
	i = 0;
//	if(memorySize % PAGEFRAMESIZE != 0){
//		memorySize = memorySize - memorySize % PAGEFRAMESIZE;
//	}
//	if((int)memoryStart % 4 != 0){
//		memoryStart = (int*)((int)memoryStart + 4 - (int)memoryStart % 4);
//	}
	//putchar('a');
	counterPageFrames = memorySize / PAGEFRAMESIZE;
	list = memoryStart;
//	putchar('a');
	while(i < counterPageFrames){
	//putchar('a');
		if(i+1 == counterPageFrames)
			*(memoryStart+i) = 0;//(int)memoryStart + PAGEFRAMESIZE;
		else
			*(memoryStart+i) = (int)memoryStart + (i+1) * PAGEFRAMESIZE;
//			putchar(i+'0');
//			putchar(10);
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
	//print((int*)"no free pages left");
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



