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

		} else if (pid > 0) {
			putchar('e');

		} else {
			exit(1);
		}


	} else {
		exit(1);
	}
}
