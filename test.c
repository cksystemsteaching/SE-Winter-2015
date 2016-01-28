
int main(){
	int pid;
	pid = getPID();
	putchar(10);
	putchar('h');
	putchar('a');
	putchar('l');
	putchar('l');
	putchar('o');
	putchar(' ');
	putchar(pid+'0');
	putchar(10);
	//yield();
	//calculateSum();
	exit(30);
}
void calculateSum(){

	int sum;
	int i;
	int max;
	max = 10000;

	i = 0;
	sum = 0;
	while(i < max){
		sum = sum + sumUp(i, max);
		i = i+1;
	}
	while(sum > 65){
		sum = sum -1;
	}
	putchar(sum+'0');

}
int sumUp(int i, int j){
	int sum;
	sum = 0;
	while(i < j){
		sum = sum + i;
		i = i+1;
	}
	return sum;
}
