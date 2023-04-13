void swap(int c, int d);

int main(){
    int a = 1;
    int b = 2;
    swap(a, b);
    return a;
}

void swap(int a, int b){
    int t = a;
    a = b;
    b = t;
}