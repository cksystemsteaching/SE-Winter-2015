//TestFile Consumer Producer
//
// Stack Structure
//                   top
//                    |
//                    v
//  | next | <---- | next |
//  | val  |       | val  |
//  If we pop from the stack, top becomes next
//
//    top
//     |
//     v
//  | next |
//  | val  |
//  If we push onto the stack,top becomes the new entry end next is the old top
//
//                                       top (new node)
//                                        |
//                                        v
//  | next |<-----| next |<-(oldTop)---| next |
//  | val  |      | val  |             | val  |
//  our emulator has some restrictions thus we have no free, so
//  we have a lot of unused/unreachable list entry's once removed from pop
//

int runs = 26;
int ascii = 65; //Start char A
int item;
int* top;
int nodeCount;
int push(int item);
int pop();
int* createNode();
int main()
{
    int pid;
    top = malloc(4);
    *top = 0;
    pid = tfork();
    nodeCount = 0;
    if (pid != 0) {
            while (runs > 0) {
                push(ascii);
            }
            exit(0);
    }
    else {
        while (1) {
            if (nodeCount <= 0) {
                if (runs <= 0) {
                    exit(pid);
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

int push(int itm)
{
    int* node;
    int old;
    node = createNode();
    while (1) {
        old = *top;
        *(node + 1) = ascii;

        if (cas(top, old, node) == 1) {
            *node = old;
            ascii = ascii + 1;
            runs = runs - 1;

            nodeCount = nodeCount + 1;

            return 1;
        }
    }
}

int pop()
{
    int newTop;
    int* oldTop;
    int old;
    int value;
    while (1) {
        oldTop = (int*)*top;
        value = *(oldTop + 1);
        old = *top;
        newTop = *oldTop;
        if (cas(top, old, newTop) == 1) {
            item = *(oldTop + 1);
            nodeCount = nodeCount - 1;
            return 1;
        }
    }
}

int* createNode()
{
    int* node;
    node = malloc(4 * 2);
    return node;
}
