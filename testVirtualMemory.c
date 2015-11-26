int main() {
	int* allocatedVMem;
	int i;
	int step;
	
	// allocate 1023 * 4 KB, in total we have 1024 * 4 KB = 4MB of virtual memory, however some of it is needed for code and stack
	allocatedVMem = malloc(4*1024*1023);

	i = 0;
	step = (4*1024*1023 / 128) / 4;
	
	while (i < 128) {
		*(allocatedVMem + i*step) = 42;
		i = i + 1;
	}

	exit(getNumberOfAllocatedPages());
}
