int main() {
	if (getPID() == 1) {
		lock();
	}

	if (getPID() == 0) {
		lock();
	}

	if (getPID() == 1) {
		lock();
	}

	if (getPID() == 1) {
		putchar('0');
		putchar('1');
		putchar('2');
	}

	if (getPID() == 0) {
		putchar('0');
		putchar('1');
		putchar('2');
	}

	if (getPID() == 2) {
		putchar('0');
		putchar('1');
		putchar('2');
	}

	if (getPID() == 1) {
		unlock();
	}

	if (getPID() == 0) {
		unlock();
	}

	if (getPID() == 1) {
		unlock();
	}

	exit(0);
}
