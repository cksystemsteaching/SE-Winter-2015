int main(){

<<<<<<< HEAD
lock();
	putchar('a');
	yield();
	putchar('b');
unlock();
	yield();
lock();
	putchar('c');
	yield();
	putchar('d');
unlock();
	yield();
lock();
	putchar('e');
	yield();
	putchar('f');
unlock();

	exit(0);
}
