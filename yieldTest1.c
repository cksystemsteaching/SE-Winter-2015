int main(){
	int counter;
	counter = 0;
	while(1){
		printAlphabet(counter);
		counter = counter + 1;
	}
	exit(0);
}

void printAlphabet(int counter){
	if(counter < 26){
		putchar('a'+counter);
		putchar('a'+counter-32);
		putchar(10);
	} else {
		exit(1);
	}
}
