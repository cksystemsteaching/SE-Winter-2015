int main(){
	int counterArgs;
	int alphabet;
	int counter;
	counterArgs = 26;
	alphabet = 'a';
	counter = 0;
	while(counter < counterArgs){
		putchar(alphabet+counter);
		putchar(10);
		yield();
		counter = counter+1;
	}
	exit(0);
}

