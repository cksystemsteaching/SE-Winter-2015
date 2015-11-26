int main(){
	int *vmemory;
	int counter;
	int counterPages;
	int pageSize;
	int pid;
	int currPID;
	pageSize = 32768-80;//Bit = 4KB - 10Byte
	counterPages = 128;
	vmemory = malloc(4194304 - 10240);//Byte = 4MB - 10KB
	counter = 0;
	
	while(counter < counterPages){
		vmemory = pageSize * counter;
		*(vmemory+counter) = 0;
		counter = counter +1;
	}
	
	pid = fork();
	currPID = getPID();
	wait(pid);
	if(pid > 0){
		putchar('c');
		putchar('h');
		putchar('i');
		putchar('l');
		putchar('d');
		putchar(' ');
		putchar(currPID+'0');
	} else if (pid == 0){
		putchar('p');
		putchar('a');
		putchar('r');
		putchar('e');
		putchar('n');
		putchar('t');
		putchar(' ');
		putchar(currPID+'0');
	} else
		exit(-1);

	putchar(10);
	exit(0);
	
}

