int* treiberStack = (int*) 0;
int* treiberStackTop = (int*) 0;

void treiberPush(int* treiberStack, int value) {
	int* node;
	int* top;

	int casSucceed;

	casSucceed = 0;

	node = malloc(2*4);

	*node = value;
	*(node + 1) = (int) 0;

	while (casSucceed == 0) {
		top = treiberStackTop;
		*(node + 1) = (int) top;

		casSucceed = (int) cas(treiberStackTop, top, node);
	}
}

int treiberPop(int* treiberStack, int* poppedValue) {
	int* top;
	int casSucceed;

	casSucceed = 0;

	while (casSucceed == 0) {
		top = treiberStackTop;

		if ((int) top == 0) {
			return 0;
		}

		casSucceed = (int) cas(treiberStackTop, top, *(top + 1));
	}

	*poppedValue = *top;

	return 1;
}

void testTreiberStack() {
	int* pop;

	pop = malloc(4);

	treiberPush(treiberStack, 23);

	treiberPush(treiberStack, 66);

	treiberPop(treiberStack, pop);

	treiberPush(treiberStack, 42);
}

int main() {
	int pid;

	pid = tfork();
	pid = tfork();
	pid = tfork();

	exit(0);
}
