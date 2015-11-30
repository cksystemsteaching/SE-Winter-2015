int main()
{
    int pid;
    putchar('h');
    putchar('a');
    putchar('l');
    putchar('l');
    putchar('o');
    pid = fork();

    if (pid == 0) {
        putchar('c');
        putchar('h');
        putchar('i');
        putchar('l');
        putchar('d');
    }
    else {
        putchar('p');
        putchar('a');
        putchar('r');
        putchar('e');
        putchar('n');
        putchar('t');
    }

    exit(0);
}
