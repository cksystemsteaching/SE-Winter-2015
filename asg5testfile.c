int main(){
    int* ptr;
    int i;
    ptr = malloc(3145728);
    i = 0;

    while(i < 3145728){
        *(ptr + i) = i;
        i = i + 31457;
    } 

}
