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
	int pid2;
	int* space;

	pid = fork();

	if (pid == 0) {
		alloc_and_access(space);

		pid = fork();

		if (pid == 0) {
			alloc_and_access(space);

			putchar('f');
			putchar('f');
			putchar('f');
			putchar('f');
			putchar('f');
			putchar('f');
			putchar('f');

			exit(1);
		} else if (pid > 0) {
			alloc_and_access(space);

			putchar('e');
			putchar('e');
			putchar('e');
			putchar('e');

			wait(pid);

			pid2 = fork();

			if (pid2 > 0) {
				putchar('i');
				putchar('i');
				putchar('i');
				putchar('i');
				wait(pid2);

				pid = fork();

				putchar('j');

				exit(5);
			} else if (pid2 == 0) {
				putchar('h');
				putchar('h');
				putchar('h');
				putchar('h');
				putchar('h');
				putchar('h');
				putchar('h');
				putchar('h');
				putchar('h');
				putchar('h');
				putchar('h');
				putchar('h');
				putchar('h');
				putchar('h');
				putchar('h');
				exit(9);
			} else
				exit(7);
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

		wait(pid);

		exit(1);
	} else {
		exit(-1);
	}

	exit(42);
}
