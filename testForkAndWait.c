int main() {
	int pid;
	int i;

	pid = fork();

	if (pid > 0){
		wait(pid); // auf bruder warten
		pid = fork();
		if (pid > 0){
			// NICHT auf schwester warten
			i = 10;
			while (i > 0) {
				putchar('O'); // opa
				i = i - 1;
			}
		} else {
			i = 10;
			while (i > 0) {
				putchar('S'); // schwester
				i = i - 1;
			}
			exit(0);
		}
	} else if (pid < 0) {
		exit(-1);
	} else {
		pid = fork();
		wait(pid); // auf enkel warten
		wait(pid); // zweiter wait macht keine probleme :-D
		if(pid > 0){
			i = 10;
			while (i > 0) {
				putchar('B'); // bruder
				i = i - 1;
			}
			exit(0);
		} else {
			i = 10;
			while (i > 0) {
				putchar('E'); // enkel
				i = i - 1;
			}
			exit(0);
		}
		
	}

	pid = fork();

	lock();

	if (pid == 0) {
		putchar('K');
	} else if (pid > 0) {
		putchar('E');
		wait(pid);
	} else {
		exit(-1);
	}

	unlock();

	exit(0);
}
