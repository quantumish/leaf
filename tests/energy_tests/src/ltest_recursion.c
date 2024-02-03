/**
 * 
 * @author bensong04
 * 
*/

int f(int n) {
    if (n < 2) return 1;
    return f(n - 1) + f(n - 2);
}

int main() {
    f(15);
    f(122);
    f(18);
    f(213);
    f(2);
    f(251);
    return 0;
}