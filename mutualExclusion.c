int main() {
	int i;
	if(getpid() == 0)
		lock();
	if(getpid() == 1)
		lock();
		
	//critical section for process 0 and process 1
	//---------------------------------------------
	if(getpid() == 0) {
		i = 10;
		while (i > 0) {
			putchar('0');
			i = i-1;
		}
	} else if(getpid() == 1) {
		i = 10;
		while (i > 0) {
			putchar('1');
			i = i-1;
		}
	} else if(getpid() == 2) {
		i = 10;
		while (i > 0) {
			putchar('2');
			i = i-1;
		}
	}
	//---------------------------------------------
	
	if(getpid() == 0)
		unlock();
	if(getpid() == 1)
		unlock();
		
	exit(0);
}
