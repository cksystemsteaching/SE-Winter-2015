//TestFile Consumer Producer

int old;
int* p;
int ret;
int pid;

int listSize = 10;
int* fillCount;
int runs = 10;
int item = 65;
int main()
{
    p = malloc(4 * listSize);
    fillCount = malloc(4);
    *fillCount = -1;
    pid = tfork();
    if (pid != 0) {
        while (runs >= 0) {
            if (*fillCount > listSize - 1) {
            }
            else {
                *fillCount = *fillCount + 1;
                *(p + fillCount) = item;
                item = item + 1;
                runs = runs - 1;
            }
        }
        exit(1);
    }
    else {
        while (1) {
            if (runs < 0) {
                if (*fillCount < 0) {
                    exit(0);
                }
            }
            else if (*fillCount == -1) {
            }
            else {
                *fillCount = *fillCount - 1;
                putchar(*(p + fillCount));
                putchar(10);
            }
        }
    }
}
