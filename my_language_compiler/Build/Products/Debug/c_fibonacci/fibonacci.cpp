#include <cstdio>

int recursive_fibonacci (int iter) {
    if (iter == 0)
        return 1;
    
    if (iter == 1)
        return 1;
    
    return recursive_fibonacci (iter - 1) + recursive_fibonacci (iter - 2);
}

int main () {
    int collector = 40;
    printf ("%d", recursive_fibonacci (collector));
}
