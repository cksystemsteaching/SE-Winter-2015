int main() {
	int counter;

	counter = 48;

	while (counter != 53) {
		putchar(counter),
		counter = counter + 1;
		sched_yield();
	}

	exit(0);
}
