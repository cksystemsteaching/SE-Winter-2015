// -----------------------------------------------------------------
// ----------------------- LIBRARY FUNCTIONS -----------------------
// -----------------------------------------------------------------

int *power_of_two_table;
int INT_MAX; // maximum numerical value of an integer
int INT_MIN; // minimum numerical value of an integer
int *string_buffer;


int twoToThePowerOf(int p) {
    // assert: 0 <= p < 31
    return *(power_of_two_table + p);
}

int leftShift(int n, int b) {
    // assert: b >= 0;

    if (b > 30)
        return 0;
    else
        return n * twoToThePowerOf(b);
}

int rightShift(int n, int b) {
    // assert: b >= 0

    if (b > 30)
        return 0;
    else if (n >= 0)
        return n / twoToThePowerOf(b);
    else
        // works even if n == INT_MIN
        return ((n + 1) + INT_MAX) / twoToThePowerOf(b) +
            (INT_MAX / twoToThePowerOf(b) + 1);
}

int loadCharacter(int *s, int i) {
    // assert: i >= 0
    int a;

    a = i / 4;

    return rightShift(leftShift(*(s + a), 24 - (i % 4) * 8), 24);
}

int* storeCharacter(int *s, int i, int c) {
    // assert: i >= 0, all characters are 7-bit
    int a;

    a = i / 4;

    *(s + a) = (*(s + a) - leftShift(loadCharacter(s, i), (i % 4) * 8)) + leftShift(c, (i % 4) * 8);

    return s;
}

void print(int *s) {
    int i;
    i = 0;

    while (loadCharacter(s, i) != 0) {
        putchar(loadCharacter(s, i));
        i = i + 1;
    }
}

void println() {
  putchar(10);
}

void initLibrary() {
    int i;

    power_of_two_table = malloc(31*4);
    *power_of_two_table = 1; // 2^0
    i = 1;

    while (i < 31) {
        *(power_of_two_table + i) = *(power_of_two_table + (i - 1)) * 2;
        i = i + 1;
    }

    // computing INT_MAX and INT_MIN without overflows
    INT_MAX = (twoToThePowerOf(30) - 1) * 2 + 1;
    INT_MIN = -INT_MAX - 1;


}


int replaceNewline(int *s, int *n) {
    int i;
    int c; 

    i = 0;

    //while (loadCharacter(s, i) != 0) {
    while (1) {
      c = loadCharacter(s, i);
      if (c == 0) {
        storeCharacter(n, i, 0);
        return; 
      } else if (c == 10) {
        storeCharacter(n, i, 0);
        return;
      }
      storeCharacter(n, i, c);
      i = i + 1;
    }

    return i;
}



int main() {
    int *a;
    int *b; 
    int pid; 
    int sz; 
    initLibrary();
    a = malloc(1024);
    b = malloc(1024);


    while(1) {

      putchar('$');   // prompt
      putchar(' ');

      sz = read(1, a, 1024);

      replaceNewline(a, b);


      pid = fork();

      if (pid == 0) {
        exec(b, 0, 0); 
      } else if (pid > 0) {
        wait(pid); 
      } else {
        exit(-1);
      }
    }
}
