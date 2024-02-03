#include <pthread.h>


int f(int n) {
    if (n < 2) return 1;
    return f(n - 1) + f(n - 2);
}

int two(int n) {
    if (n < 2) return 1;
    return two(n - 1) + f(n - 2);
}

void* thread1(void*) {
    f(15);
    f(122);
    f(18);
    f(213);
    f(2);
    f(251);
}

void* thread2(void*) {
    two(15);
    two(122);    
    f(18);
    two(213);
    f(2);
    two(251);
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}
