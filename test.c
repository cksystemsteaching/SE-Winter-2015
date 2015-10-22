int main() {
  int a;
  int pid;
  a = add(100);
  pid = getpid();
  putchar(pid+'0');
  putchar(10);
  exit(a);
}

int add(int n) {
  if (n == 0)
    return 0;
  else 
    return add(n-1) + n;
}
