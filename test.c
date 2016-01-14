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

  if (pid > 0) {
	wait(pid);

	putchar('z');

	exit(7);
  } else {

        putchar('k');
        putchar('k');
        putchar('k');
        putchar('k');
        putchar('k');
        putchar('k');
        putchar('k');

  	exit(5);
  }
}
