void alloc_and_access(int* space) {
	int size;
	int counter;

	counter = 0;

	size = 1024*1024*4;
	space = malloc(size);

	while (counter < 128) {
		*((int) space + size*(counter/128)) = 42;
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
		} else if (pid > 0) {
			alloc_and_access(space);
		} else {
			exit(-1);
		}
	} else if (pid > 0) {
		alloc_and_access(space);
	} else {
		exit(-1);
	}
}
