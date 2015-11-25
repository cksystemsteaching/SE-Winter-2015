void alloc_and_access(int* space) {
	int size;
	int counter;

	counter = 0;

	size = 1024*4;
	space = malloc(size * 511);

	while (counter < 128) {
		*((int) space + size*counter) = 42;
		counter = counter + 1;
	}
}

int main() {
	int pid;
	int* space;

	pid = fork();

	if (pid == 0) {
		alloc_and_access(space);

		pid = fork();

		if (pid == 0) {
			alloc_and_access(space);
			exit(2);
		} else if (pid > 0) {
			alloc_and_access(space);
			wait(pid);
			exit(3);
		} else {
			exit(-1);
		}
	} else if (pid > 0) {
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

		alloc_and_access(space);

		wait(pid);

		exit(4);
	} else {
		exit(-1);
	}

	exit(42);
}
