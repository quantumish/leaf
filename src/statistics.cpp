/**
 * 
 * @author bensong04
 * 
*/

#include "statistics.hpp"
#include "rapl.hpp"

/**
 * 
 * Source: https://www.johndcook.com/blog/normal_cdf_inverse/
 * 
*/
double RationalApproximation(double t)
{
    // Abramowitz and Stegun formula 26.2.23.
    // The absolute value of the error should be less than 4.5 e-4.
    double c[] = {2.515517, 0.802853, 0.010328};
    double d[] = {1.432788, 0.189269, 0.001308};
    return t - ((c[2]*t + c[1])*t + c[0]) /
        (((d[2]*t + d[1])*t + d[0])*t + 1.0);
}

/**
 * 
 * Source: https://www.johndcook.com/blog/normal_cdf_inverse/
 * 
*/
double NormalCDFInverse(double p)
{
    if (p <= 0.0 || p >= 1.0)
    {
        return -1.0; // should never return this value
    }
    else if (p < 0.5)
    {
        // F^-1(p) = - G^-1(p)
        return -RationalApproximation( std::sqrt(-2.0*std::log(p)) );
    }
    else
    {
        // F^-1(p) = G^-1(1-p)
        return RationalApproximation( std::sqrt(-2.0*std::log(1-p)) );
    }
}

std::pair<double, double> record_baseline_conf_interval(int iterations, double confidence, uint32_t& last_rapl) {
    std::vector<uint32_t> baseline_data_in_mj;
    uint64_t mean_mj = 0;
    for (int iteration = 0; iteration < iterations; iteration++) {
        uint32_t mj = get_curr_mjoule_usage(last_rapl);
        baseline_data_in_mj.push_back(mj);
        mean_mj += mj;
        SLEEP(1); // sleep for one ms to allow RAPL to update
    }
    mean_mj /= iterations;
    double variance = 0;
    for (int iteration_idx = 0; iteration_idx < iterations; iteration_idx++) {
        variance += std::pow((mean_mj - baseline_data_in_mj[iteration_idx]), 2);
    }
    variance /= (iterations - 1);
    double stdev = std::sqrt(variance);
    // return degenerate interval in case confidence is invalid
    if (confidence < 0 || confidence > 1) return std::pair<double, double>(0, 0); 
    // calculate inverse normal CDF (z*)
    double inv_norm_cdf = NormalCDFInverse(0.5 + confidence/2);
    double conf_width = inv_norm_cdf * stdev / std::sqrt(iterations);
    return std::pair<double, double>(mean_mj - conf_width, mean_mj + conf_width);
}

std::pair<double, double> conf_interval_from_samples(std::vector<uint32_t> samples, double confidence) {
    size_t iterations = samples.size();
    uint64_t mean_mj = 0;
    for (size_t iteration = 0; iteration < iterations; iteration++) {
        mean_mj += samples[iteration];
    }
    mean_mj /= samples.size();
    double variance = 0;
    for (size_t iteration_idx = 0; iteration_idx < iterations; iteration_idx++) {
        variance += std::pow((mean_mj - samples[iteration_idx]), 2);
    }
    variance /= (iterations - 1);
    double stdev = std::sqrt(variance);
    // return degenerate interval in case confidence is invalid
    if (confidence < 0 || confidence > 1) return std::pair<double, double>(0, 0); 
    // calculate inverse normal CDF (z*)
    double inv_norm_cdf = NormalCDFInverse(0.5 + confidence/2);
    double conf_width = inv_norm_cdf * stdev / std::sqrt(iterations);
    return std::pair<double, double>(mean_mj - conf_width, mean_mj + conf_width);
}