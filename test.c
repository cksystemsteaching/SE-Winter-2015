int main() {
	int pid;
	int a;

	pid = fork();

	if (pid == 0) {
		a = 42;
		putchar('a');
		putchar('a');
		putchar('a');
		putchar('a');
		putchar('a');
		putchar('a');
		putchar('a');
		putchar('a');
		putchar('a');
		putchar('a');
		putchar('a');
		putchar('a');
		putchar('a');
		putchar('a');
		putchar('a');
		putchar('a');
		putchar('a');
		putchar('a');
		putchar('a');
		putchar('a');
		putchar('a');
		putchar('a');
		putchar('a');
		putchar('a');
		putchar(' ');
		exit(2);
	} else if (pid > 0) {
		putchar('b');
		putchar('b');
		putchar('b');
		putchar('b');
		putchar(' ');
		wait(pid);
		putchar('c');
		exit(3);
	} else {
		exit(1);
	}
}
