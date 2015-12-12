//========================================
//Team USEG Microkernel Server
//Kawalar Christian
//Schlager Daniel
//Mayer Alexander
//========================================
int *args;
int main(){
    args = malloc(4*4);
    *(args) = 12;
    if(*args == 12){
        putchar('a');

    }else{
        putchar('b');
    }
    exit(*args);
}
