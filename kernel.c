//--- variables ---

int *currProcess;
int *currPageTable;
int *processQueue;


// --- constants ---
int PAGESIZE = 4096;		// Byte, 4KB
int PAGEFRAMESIZE = 4096;	// Byte, 4KB
int VMEMORYSIZE = 4194304;	// Byte, 4MB
int CREATECONTEXT = 1;
int SWITCHCONTEXT = 2;
int DELETECONTEXT = 3;
int MAPPAGEINCONTEXT = 4;
int FLUSHPAGEINCONTEXT = 5;


// --- methods ---
int* createProcess();
int* initList();
int* findProcessByPid(int *queue, int pid);
void deleteProcess(int *queue, int pid);
void appendProcess(int *queue, int *process);
int* pollHead(int *queue);
int* pollTail(int *queue);
void printProcessQueue(int *queue);

void setPrevProcess(int *process, int *prev);
void setNextProcess(int *process, int *next);
void setPid(int *process, int pid);

int* getPrevProcess(int *process);
int* getNextProcess(int *process);
int  getPid(int *process);

int main(){
	int pid;
//	int *process;
	int *ipc;
	ipc = allocIpc(4);
	processQueue = initList();
	currProcess = createProcess();
	hcLoadBinary(getPid(currProcess));
//	putchar('p');
//	putchar('i');
//	putchar('d');
//	putchar(' ');
//	putchar(getPid(currProcess)+'0');
//	putchar(10);
	hcSwitchContext(getPid(currProcess));
//	exit(10);
	
	putchar(10);
	putchar('K');
	putchar('E');
	putchar('R');
	putchar('N');
	putchar('E');
	putchar('L');
	putchar(10);
	putchar(10);
	
	while(1){
		putchar(10);
		putchar('W');
		putchar('H');
		putchar('I');
		putchar('L');
		putchar('E');
		putchar(10);

	
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
			putchar('p');
			putchar('i');
			putchar('d');
			putchar(' ');
			putchar(getPid(currProcess)+'0');
			putchar(10);
		
		} else if(*ipc == SWITCHCONTEXT){
			putchar('s');
			putchar('w');
			putchar('i');
			putchar('t');
			putchar('c');
			putchar('h');
			putchar(10);
			//printProcessQueue(processQueue);
			hcSwitchContext(getPid(currProcess));
			exit(50);
		} else if(*ipc == DELETECONTEXT){
			pid = getPid(currProcess);
			putchar('d');
			putchar('e');
			putchar('l');
			putchar('e');
			putchar('t');
			putchar('e');
			putchar(10);
			deleteProcess(processQueue, pid);
			exit(99);
		} else if(*ipc == MAPPAGEINCONTEXT){
			putchar('m');
			putchar('a');
			putchar('p');
			putchar(10);
		
		} else if(*ipc == FLUSHPAGEINCONTEXT){
			putchar('f');
			putchar('l');
			putchar('u');
			putchar('s');
			putchar('h');
			putchar(10);
		
		} else {
			putchar(10);
			putchar('e');
			putchar('l');
			putchar('s');
			putchar('e');
			putchar(10);
		
		}
	}
	
	
	exit(20);
}


int* createProcess(){
	int *process;
	int pid;

	process = malloc(3*4);
	pid = hcCreateContext();

//	putchar('p');
//	putchar('i');
//	putchar('d');
//	putchar(' ');
//	putchar(pid+'0');
//	putchar(10);
	setPid(process, pid);

	appendProcess(processQueue, process);
	return process;
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
void deleteProcess(int *queue, int pid){
	int *prev;
	int *next;
	int *process;
	process = findProcessByPid(processQueue, pid);
	if(*queue == 0)
		return;
	if((int)process == 0)
		return (int*)0;
	if(getPid(process) == getPid((int*)*queue)){		// process is first element
		next = getNextProcess(process);
		if((int)next == 0){			// process is first and last element
			*queue = 0;
			*(queue+1) = 0;
		} else {					// process is first element but not last element
			*next = 0;
			*queue = (int)next;
		}
	} else if(getPid(process) == getPid((int*)*(queue+1))){ // process is not first but last element
		prev = getPrevProcess(process);
		*(prev+1) = 0;
		*(queue+1) = (int)prev;
	} else {						// process is in middle of queue
		prev = getPrevProcess(process);
		next = getNextProcess(process);
		*(prev+1) = (int)next;
		*next = (int)prev;
	}
	
	//hcDeleteContext(pid);
}
void appendProcess(int *queue, int *process){
	int *tail;
	tail = pollTail(queue);
	if((int)tail == 0){
		*queue = (int)process;
		*(queue+1) = (int)process;
	} else {
		setNextProcess(tail, process);
		*(queue+1) = (int)process;
	}
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
	putchar('p');
	putchar('i');
	putchar('d');
	putchar(':');
	putchar(' ');
	while((int)curr != 0){
		putchar(getPid(curr));
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



