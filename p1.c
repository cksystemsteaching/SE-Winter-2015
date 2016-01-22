//TestFile for asg 6 and 7
void fork(int pId);

int main(){
     fork(0);
    
}

void fork(int pid){
    pid = tfork();
    putchar('a');

    if(pid != 0){
        putchar('P');
        exit(pid);
    }else{
        putchar('C');
        exit(pid);
    }
}
