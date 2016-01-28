int* ts;

void push(int* stack, int value) {
	int* node;
	int* top;
	
	node = malloc(2 * 4);
	*(node + 1) = value;
	*node = 0;
	
	top = *stack;
	*node = *top;

	while (CAS(stack, top, node) == 0) {
		top = *stack;
		*node = *top;
	}
}

int pop(int* stack, int* pvalue) {
	int* top;
	
	top = *stack;
	
	if (top == (int*) 0) {
		return 0;
	}
	
	while (CAS(stack, top, *top) == 0) {
		top = *stack;
	
		if (top == (int*) 0) {
			return 0;
		}
	}
	
	*pvalue = *(top + 1);
	
	return 1;
}

int main() {
	int pid;
	int i;
	int j;
	int k;
	int* pvalue;
	int b;
	
	ts = malloc(4);
	*ts = 0;

	pid = threadfork();

	if (pid == -1)
		exit(-1);
	else if (pid > 0){//main thread - consumer
		wait(pid);	
		
		i = 0;
		pvalue = malloc(2 * 4);
		while (i < 5) {
			b = pop(ts, pvalue);
			//while(b == 0) {
				//b = pop(ts, pvalue);
			//}
			putchar(*pvalue);
			i = i+ 1;
		}
		
			
	} else { //thread 1 - producer
		i = 0;

		while (i < 5) {
			push(ts, 97 + i);
			i = i + 1;
		}

		exit(0);
	}
	
	exit(0);
}
