int main() {

  int* a;

  a = malloc(4096);

  *(a+1000) = 1;

  exit(5);

}
