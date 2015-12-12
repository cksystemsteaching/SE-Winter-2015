//========================================
//Team USEG Microkernel Server
//Kawalar Christian
//Schlager Daniel
//Mayer Alexander
//========================================
//=======================================//
//                                       //
//         Global Variables              //
//                                       // 
//=======================================//

int *selfieName = (int*) 0;
int *outputName = (int*) 0;
int outputFD    = 1;
int *args;
int *character_buffer;

//=======================================//
//                                       //
//         Library Functions             //
//                                       // 
//=======================================//
void initLibrary();
int* itoa(int n, int *s, int b, int a, int p);
void print(int *s);
void println();
void putCharacter(int character);
int twoToThePowerOf(int p); 
int* storeCharacter(int *s, int i, int c); 
int *power_of_two_table;
int leftShift(int n, int b);
int rightShift(int n, int b);
int loadCharacter(int *s, int i);
int  stringLength(int *s);
void stringReverse(int *s);
int  stringCompare(int *s, int *t);


int INT_MAX;
int INT_MIN;


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
    character_buffer = malloc(1);
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
        // works even if n == INT_MIN:
        // shift right n with msb reset and then restore msb
        return ((n + 1) + INT_MAX) / twoToThePowerOf(b) +
            (INT_MAX / twoToThePowerOf(b) + 1);
}
int* itoa(int n, int *s, int b, int a, int p) {
    // assert: b in {2,4,8,10,16}

    int i;
    int sign;
    int msb;

    i = 0;

    sign = 0;

    msb = 0;

    if (n == 0) {
        storeCharacter(s, 0, '0');

        i = 1;
    } else if (n < 0) {
        sign = 1;

        if (b == 10) {
            if (n == INT_MIN) {
                // rightmost decimal digit of 32-bit INT_MIN
                storeCharacter(s, 0, '8');

                // avoids overflow
                n = -(n / 10);
                i = 1;
            } else
                n = -n;
        } else {
            if (n == INT_MIN) {
                // rightmost non-decimal digit of INT_MIN
                storeCharacter(s, 0, '0');

                // avoids setting n to 0
                n = (rightShift(INT_MIN, 1) / b) * 2;
                i = 1;
            } else {
                // reset msb, restore below
                n   = rightShift(leftShift(n, 1), 1);
                msb = 1;
            }
        }

        // assert: n > 0
    }

    while (n != 0) {
        if (p > 0)
            if (i == p) {
                storeCharacter(s, i, '.'); // set point of fixed point number

                i = i + 1;
                p = 0;
            }

        if (n % b > 9)
            storeCharacter(s, i, n % b - 10 + 'A');
        else
            storeCharacter(s, i, n % b + '0');

        n = n / b;
        i = i + 1;

        if (msb == 1) {
            // restore msb from above
            n   = n + (rightShift(INT_MIN, 1) / b) * 2;
            msb = 0;
        }
    }

    if (p > 0) {
        while (i < p) {
            storeCharacter(s, i, '0'); // no point yet, fill with 0s

            i = i + 1;
        }

        storeCharacter(s, i, '.');     // set point
        storeCharacter(s, i + 1, '0'); // leading 0

        i = i + 2;
        p = 0;
    }

    if (b == 10) {
        if (sign) {
            storeCharacter(s, i, '-'); // negative decimal numbers start with -

            i = i + 1;
        }

        while (i < a) {
            storeCharacter(s, i, ' '); // align with spaces

            i = i + 1;
        }
    } else {
        while (i < a) {
            storeCharacter(s, i, '0'); // align with 0s

            i = i + 1;
        }

        if (b == 8) {
            storeCharacter(s, i, '0');     // octal numbers start with 00
            storeCharacter(s, i + 1, '0');

            i = i + 2;
        } else if (b == 16) {
            storeCharacter(s, i, 'x');     // hexadecimal numbers start with 0x
            storeCharacter(s, i + 1, '0');

            i = i + 2;
        }
    }
    
    storeCharacter(s, i, 0); // null terminated string

    stringReverse(s);

    return s;
}

int stringCompare(int *s, int *t) {
    int i;

    i = 0;

    while (1)
        if (loadCharacter(s, i) == 0)
            if (loadCharacter(t, i) == 0)
                return 1;
            else
                return 0;
        else if (loadCharacter(s, i) == loadCharacter(t, i))
            i = i + 1;
        else
            return 0;
}

void print(int *s) {
    int i;

    i = 0;
    while (loadCharacter(s, i) != 0) {
        putchar(loadCharacter(s, i));

        i = i + 1;
    }
}

void putCharacter(int character) {
    if (outputFD == 1)
        putchar(character);
    else {
        *character_buffer = character;

        if (write(outputFD, character_buffer, 1) != 1) {
            outputFD = 1;

            print(selfieName);
            print((int*) ": could not write character to output file ");
            print(outputName);
            println();

            exit(-1);
        }
    }
}

void println() {
    putCharacter(10);
}

int twoToThePowerOf(int p) {
    // assert: 0 <= p < 31
    return *(power_of_two_table + p);
}

int* storeCharacter(int *s, int i, int c) {
    // assert: i >= 0, all characters are 7-bit
    int a;

    a = i / 4;

    *(s + a) = (*(s + a) - leftShift(loadCharacter(s, i), (i % 4) * 8)) + leftShift(c, (i % 4) * 8);
    
    return s;
}

int stringLength(int *s) {
    int i;

    i = 0;

    while (loadCharacter(s, i) != 0)
        i = i + 1;

    return i;
}
void stringReverse(int *s) {
    int i;
    int j;
    int tmp;

    i = 0;
    j = stringLength(s) - 1;

    while (i < j) {
        tmp = loadCharacter(s, i);
        
        storeCharacter(s, i, loadCharacter(s, j));
        storeCharacter(s, j, tmp);

        i = i + 1;
        j = j - 1;
    }
}

int loadCharacter(int *s, int i) {
    // assert: i >= 0
    int a;

    a = i / 4;

    return rightShift(leftShift(*(s + a), 24 - (i % 4) * 8), 24);
}

int main(){
    initLibrary();
    args = amalloc(4*4,2);
    print((int*)"b");
    println();
    if(*args == 12){
        putchar('a');

    }else{
        putchar('b');
    }
    exit(*args);
}
