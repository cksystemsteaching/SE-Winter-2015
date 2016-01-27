int main(){
	putchar(10);
	putchar(10);
	putchar(10);
	putchar('h');
	putchar(10);
	putchar(10);
	putchar('a');
	putchar(10);
	putchar(10);
	putchar('l');
	putchar(10);
	putchar(10);
	putchar('l');
	putchar(10);
	putchar(10);
	putchar('o');
	putchar(10);
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

