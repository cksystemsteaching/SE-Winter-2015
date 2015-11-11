int main(){
    int pid;
    pid = fork();
    
    if(pid == 0){
        putchar('b');
    }else{
        putchar('a');
    }
    exit(getpid());
}
