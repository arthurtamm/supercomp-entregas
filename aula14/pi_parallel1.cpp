#include <iostream>
#include <random>
#include <iomanip>
#include <omp.h>

int main() {
    int N = 10e6; 
    int inside_circle = 0;

    double start_time = omp_get_wtime();

    // Um gerador compartilhado para todas as threads
    std::mt19937 gen(static_cast<unsigned int>(std::time(0)));
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    #pragma omp parallel reduction(+:inside_circle)
    {
        #pragma omp for
        for (int i = 0; i < N; i++) {
            double x, y;

            // Protege o sorteio para evitar race conditions
            #pragma omp critical
            {
                x = dist(gen);
                y = dist(gen);
            }

            if (x * x + y * y <= 1) {
                inside_circle++;
            }
        }
    }

    double pi = 4.0 * inside_circle / N;
    double end_time = omp_get_wtime();

    std::cout << std::fixed << std::setprecision(15) << "pi = " << pi << std::endl;
    std::cout << "Time: " << end_time - start_time << " seconds" << std::endl;

    return 0;
}
