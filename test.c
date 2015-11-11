int main() {
	int pid;
	int a;

	pid = fork();

	if (pid == 0) {
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

		lock();

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
			unlock();
		} else if (pid > 0) {
			putchar('e');
			putchar('e');
			putchar('e');
			putchar('e');
			unlock();
			wait(pid);
			putchar('f');
		} else {
			exit(1);
		}

		exit(5);

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
