int main() {
  int a; 

  a = 1; 

  while (a < 5){
     putchar(a + '0');
     putchar(10);
     yield();
     a = a + 1; 
  }
  exit(0);
}
