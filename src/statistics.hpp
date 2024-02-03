/**
 * 
 * Suite of statistical tools for converting raw data into nice, readable plots.
 * 
 * @author bensong04
 * 
*/

#include <utility>
#include <vector>
#include <numeric>
#include <cmath>

double NormalCDFInverse(double p);

std::pair<double, double> record_baseline_conf_interval(int iterations, double confidence, uint32_t& last_rapl);