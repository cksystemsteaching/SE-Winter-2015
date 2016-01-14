int main() {
    int a;

    a = fork();

    if (a == 0) {
        putchar('f');
        putchar('o');
        putchar('r');
        putchar('k');
        putchar(10);
    } else {
        wait(a);
    }
    exit(9);
}
