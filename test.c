int main(){
	int pid;
	pid = getPID();
	putchar(10);
	putchar('h');
	putchar('a');
	putchar('l');
	yield();
	putchar('l');
	putchar('o');
	putchar(' ');
	putchar(pid+'0');
	putchar(10);
	exit(30);
}

