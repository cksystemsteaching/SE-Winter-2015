

int main() {
  int pid;
  int a;

  a = 1;
   
  while (a < 4) {
    pid = fork();
  
    if (pid == 0) {
      putchar('I');
      putchar(39);
      putchar('m');
      putchar(' ');
      putchar('c');
      putchar('h');
      putchar('i');
      putchar('l');
      putchar('d');
      putchar(' ');
      putchar(getpid()+'0');
      putchar(10);
      a = 3;

    } else if (pid > 0) {
      wait(pid);
      putchar('I');
      putchar(39);
      putchar('m');
      putchar(' ');
      putchar('p');
      putchar('a');
      putchar('r');
      putchar('e');
      putchar('n');
      putchar('t');
      putchar(' ');
      putchar(getpid()+'0');
      putchar(' ');
      putchar('o');
      putchar('f');
      putchar(' ');
      putchar(pid+'0');
      putchar(10);
  
    } else {
      exit(-1);
    }
    a = a + 1;
  }

  exit(pid);
}
