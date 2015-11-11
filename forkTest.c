int main(){
	int pid;
	int currentProcessID;
	int counter;
	counter = 0;
	pid = fork(); // pid = childpid
	currentProcessID = getPID();

	putchar(10);
	putchar('c');
	putchar('u');
	putchar('r');
	putchar('r');
	putchar(' ');
	putchar('p');
	putchar('r');
	putchar('o');
	putchar('c');
	putchar('e');
	putchar('s');
	putchar('s');
	putchar(' ');
	putchar(currentProcessID+'0');
	putchar(' ');
	putchar(10);
	if(pid < 0){
		// i am parent, no child was created
		// do nothing
		putchar('p');
		putchar('i');
		putchar('d');
		putchar('<');
		putchar('0');
		putchar(10);
	} else if(pid > 0){
		// i am parent, child exists with PID = 'pid'
		// wait for child
		// do something
		putchar('p');
		putchar('a');
		putchar('r');
		putchar('e');
		putchar('n');
		putchar('t');
		putchar(' ');
		putchar('o');
		putchar('f');
		putchar(' ');
		putchar(pid+'0');
		putchar(10);
	} else { // pid == 0
		// i am child
		// do something
		putchar('c');
		putchar('h');
		putchar('i');
		putchar('l');
		putchar('d');
		putchar(' ');
		putchar('o');
		putchar('f');
		putchar(' ');
		putchar(pid+'0');
		putchar(10);
	}
	
	exit(0);
}
