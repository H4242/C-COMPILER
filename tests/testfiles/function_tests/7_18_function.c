int add(int a, int b) {
    return a + b;
}

int main() {
    int a = 1;
    int b = 2;
    int c = add(a, add(a,b+a|a*(a-b)));
    return c;
}