int main(){
	int counter;
	counter = 0;
	while(1){
		printAlphabet(counter);
		counter = counter + 1;
	}
	exit(0);
}

int printAlphabet(int counter){
	if(counter < 10){
	lock();
		putchar('a'+counter);
		yield();
		putchar('a'+counter-32);
		yield();
		putchar(10);
	unlock();
		yield();
	} else {
		exit(0);
	}
}
