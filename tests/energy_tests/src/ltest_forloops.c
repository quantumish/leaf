/**
 * 
 * @author bensong04
 * 
*/

#include <stdio.h>

void f1() {
    int res = 1;
    for (int i = 0; i < 1000; i++) {
        res += i;
    }
}

void f2() {
    int res = 1;
    for (int i = 0; i < 10; i++) {
        res += i;
    }
}

void f3() {
    int res = 1;
    for (int i = 0; i < 100; i++) {
        res += i;
    }
}

void f4() {
    int res = 1;
    f1();
    f3();
}

int main() {
    f1();
    f2();
    f3();
    f4();
    return 0;
}