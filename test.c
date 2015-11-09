int main() {
	int pid;
	int a;

	pid = fork();

	if (pid == 0) {
		a = 42;
		putchar('a');
		exit(0);
	} else if (pid > 0) {
		putchar('b');
		wait(pid);
		exit(0);
	} else {
		// ?
	}
}
