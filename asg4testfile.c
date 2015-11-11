int main(){
    int pid;
    pid = fork();
    
    if(pid == 0){
        putchar('b');
        putchar(10);
    }else{
        putchar('a');
        putchar(10);
    }
    exit(0);
}
