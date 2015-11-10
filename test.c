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

		pid = fork();

		if (pid == 0) {
			putchar('d');
			putchar('d');
			putchar('d');
			putchar('d');
			putchar('d');
			putchar('d');
			putchar('d');
			putchar('d');
			putchar('d');
			putchar('d');
			putchar('d');
			putchar('d');
			exit(5);			
		} else if (pid > 0) {
			putchar('e');
			putchar('e');
			putchar('e');
			putchar('e');
			wait(pid);
			exit(6);			
		} else {
			exit(1);
		}
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
