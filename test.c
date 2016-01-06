int main() {

  int* a;

  a = malloc(4096);

  *(a+1000) = 1;

  *(a+2000) = 1;

  putchar('h');
  putchar('e');
  putchar('l');
  putchar('l');
  putchar('o');
  putchar(' ');

  exit(5);

}
