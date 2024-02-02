#include <stdio.h>
#include "rapl.h"

int main() {
    uint32_t usage = get_curr_mjoule_usage();
    printf("%u\n", usage);
    return 0;
}
