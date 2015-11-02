int main() {
	int counter;

	counter = 48;

	while (counter != 50) {
		putchar(counter);
		counter = counter + 1;
	}

	lock();

	while (counter != 55) {
		putchar(counter);
		counter = counter + 1;
	}

	unlock();

	while (counter != 56) {
		putchar(counter);
		counter = counter + 1;
	}

	exit(0);
}
