int main() {
  int *a; 
  int i; 
  int pid; 
  a = malloc(4186112);  // allocate ~4MB

  i = 0; 
  while (i < 128) {    // access 128 pages, this will result in 128 real page frames.  
    a = i * 32704;
    *(a + i) = 0; 
    i = i + 1;
  }

  pid = fork();        // fork is also possible
  if (pid < 0) {
     exit(-1);
  } else if(pid == 0) {
     exit(pid);        // child
  } else {
     exit(pid);
  }
}
