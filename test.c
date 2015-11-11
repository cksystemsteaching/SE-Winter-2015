int main() {
	int pid;
	int a;

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
		putchar('d');
		putchar('d');
		putchar('d');
		exit(4);
	} else if (pid > 0) {
		pid = fork();

		if (pid == 0) {
			putchar('e');
			putchar('e');
			putchar('e');
			putchar('e');
			putchar('e');
			putchar('e');
			exit(6);
		} else {
			putchar(' ');
			putchar('c');
			wait(pid);
			exit(3);
		}
	} else {
		exit(1);
	}
}
