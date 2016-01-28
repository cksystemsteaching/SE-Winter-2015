
int pop_value;
int *top;

int* createNode(int value);
int push(int value);
int pop();


int main() {
    int i;
    int pid;
    int j;

    shmat();
    top = createNode(0);

    pid = fork();

    if (pid == 0) {
        i = 0;
        while(i < 10) {
            if(push(i)) {
                lock();
                putchar('p');
                putchar('u');
                putchar('s');
                putchar('h');
                putchar(' ');
                putchar(i + '0');
                putchar(10);
                unlock();
            }
            i = i + 1;
        }

    } else {
        i = 0;
        while(1) {
            if (pop()) {
                lock();
                putchar('p');
                putchar('o');
                putchar('p');
                putchar(' ');
                putchar(pop_value+'0');
                putchar(10);
                unlock();
            }
            i = i + 1;

            if (i > 100)
                exit(0);
            // pops as long something is on the stack,
            // but definitely leaves after 100 cycles.
        }
    }
}

int* createNode(int value) {
    int *node;

    node = shmalloc(2*4);

    *(node+1) = 0;       //node->next
    *(node+1) = value;   //node->value

    return node;
}

int push(int value) {
    int *expected;
    int *node;

    node = createNode(value);

    if (node == (int*) 0)
        exit(-1);

    *node = *top;

    expected = top;

    while(CAS(top, *expected, node, (*(top+1) + 1)) == 0) {
        expected = top;
    }

    return 1;
}

int pop() {
    int *expected;
    int *next;
    int *first_element;

    if (*top == 0)
        return 0;

    expected = top;

    first_element = (int*)*top;

    next = (int*)*expected;

    while(CAS(top, *expected, *next, (*(top+1) - 1)) == 0) {
        if (*top == 0)
            return 0;

        first_element = (int*)*top;
        expected = top;
    }

    pop_value = *(first_element+1);

    return 1;
}