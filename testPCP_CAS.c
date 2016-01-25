//TestFile Consumer Producer

int old;
int* p;
int ret;

int listSize = 10; //Our Array has max 10 entrys
int fillCount;
int runs = 10;
int ascii = 65; //Start char A
int item;
int* mutex; //We use a mutex with cas to ensure that only one process can read and write at a time
            //My understanding is that this is also a Treiber Stack, because it's a lock free implementation
int push(int item);
int pop();

int main()
{
    int pid;
    p = malloc(4 * listSize);
    mutex = malloc(4);
    *mutex = 0;
    fillCount = -1;
    pid = tfork();

    if (pid != 0) {
        while (runs >= 0) {
            if (fillCount > listSize - 1) {
            }
            else {
                if (push(ascii) == 1) {
                    runs = runs - 1;
                }
            }
        }
        exit(0);
    }
    else {
        while (1) {
            if (fillCount < 0) {
                if(runs < 0){
                    exit(0);
                }
            }
            else {
                if (pop() == 1) {
                    putchar(item);
                    putchar(10);
                }
            }
        }
    }
}

int push(int item)
{
    if (cas(mutex, 0, 1) == 1) {
        fillCount = fillCount + 1;
        *(p + fillCount) = item;
        ascii = ascii + 1;
        *mutex = 0; //Done
        return 1;
    }
    return 0;
}

int pop()
{
    if (cas(mutex, 0, 1) == 1) {
        item = *(p + fillCount);
        fillCount = fillCount - 1;
        *mutex = 0;
        return 1;
    }
    return 0;
}
