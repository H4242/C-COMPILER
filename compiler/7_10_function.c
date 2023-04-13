int swap(int a, int b);

int main(){
    int a = 1;
    int b = 2;
    int c = 3;
    swap(a, b, c);
    return a;
}

int swap(int a, int b){
    int t = a;
    a = b;
    b = t;
}