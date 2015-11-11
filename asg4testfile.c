int main(){
    int pid;
    pid = fork();
    
    if(pid == 0){
        putchar('c');
        putchar('h');
        putchar('i');
        putchar('l');
        putchar('d');
    }else{
        putchar('p');
        putchar('a');
        putchar('r');
        putchar('e');
        putchar('n');
        putchar('t');
        
    }

    exit(0);
}
