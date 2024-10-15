#include <omp.h>
#include <iostream>
#include <iomanip>

static long num_steps = 1024l * 1024 * 1024 * 2;
double sum = 0;

void pi_r_parallel_for(long Nstart, long Nfinish, double step, long MIN_BLK) {
    long i, iblk;
    if (Nfinish - Nstart < MIN_BLK) {
        #pragma omp parallel for reduction(+:sum)
        for (i = Nstart; i < Nfinish; i++) {
            double x = (i + 0.5) * step;
            sum += 4.0 / (1.0 + x * x);
        }
    } else {
        iblk = Nfinish - Nstart;
        pi_r_parallel_for(Nstart, Nfinish - iblk / 2, step, MIN_BLK);
        pi_r_parallel_for(Nfinish - iblk / 2, Nfinish, step, MIN_BLK);
    }
}

int main() {
    long i;
    double step, pi;
    double init_time, final_time;
    step = 1.0 / (double) num_steps;
    long MIN_BLK_values[] = {1024l, 512l, 256l};
    std::cout << "Pi Parallel For\n";

    for (int blk = 0; blk < 3; blk++) {
        sum = 0;  // Resetar a soma antes de cada execução
        init_time = omp_get_wtime();
        pi_r_parallel_for(0, num_steps, step, MIN_BLK_values[blk]);
        pi = step * sum;
        final_time = omp_get_wtime() - init_time;

        std::cout << "MIN_BLK: " << MIN_BLK_values[blk] << "\n";
        std::cout << "for " << num_steps << " steps pi = " << std::setprecision(15) << pi << " in " << final_time << " secs\n";
    }
    return 0;
}
