#include <cstdio>
#include "statistics.hpp"

int main() {
    std::pair<double, double> conf_int = record_baseline_conf_interval(1500, 0.95);
    printf("Baseline: (%lf, %lf) mJ with 0.95 confidence.\n", conf_int.first, conf_int.second);
    return 0;
}