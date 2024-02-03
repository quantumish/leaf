#include <cstdio>
#include "statistics.hpp"

int main() {
    double test = NormalCDFInverse(0.975);
    printf("z* = %lf\n", test);
    std::pair<double, double> conf_int = record_baseline_conf_interval(1500, 0.95);
    printf("Baseline: (%lf, %lf) mJ with 0.95 confidence.\n", conf_int.first, conf_int.second);
    return 0;
}