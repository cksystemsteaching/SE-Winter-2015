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

  if( pid == 0) {

    pid = fork()

    if ( pid == 0) {
      exit(20);
    }
    exit(10);
  }

  exit(5);

}
