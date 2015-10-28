int main() {
	int counter;
	
	counter = 48;
	
	while(counter < 58) {
		putchar(counter);
		yield();
		counter = counter + 1;
	} 
	
	exit(0);
}
