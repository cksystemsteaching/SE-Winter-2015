int main() {

  int* a;
  int pid;

  a = malloc(4096);

  *(a+1000) = 1;

  *(a+2000) = 1;

  putchar('h');
  putchar('e');
  putchar('l');
  putchar('l');
  putchar('o');
  putchar(' ');

  pid = fork();

  lock();

  putchar('l');
  putchar('o');
  putchar('c');
  putchar('k');

  unlock();

  putchar('t');
  putchar('e');
  putchar('s');
  putchar('t');

  if (pid > 0) {
	wait(pid);

	putchar('z');

	pid = fork();

	if (pid > 0) {
		wait(pid);
		exit(24);
	}

	putchar('i');
	putchar('i');
	putchar('i');
	putchar('i');
	putchar('i');

	exit(7);
  } else {
        putchar('k');
        putchar('k');
        putchar('k');
        putchar('k');
        putchar('k');

	lock();

        putchar('k');

	unlock();

        putchar('k');
        putchar('k');
        putchar('k');
        putchar('k');

  	exit(5);
  }
}
