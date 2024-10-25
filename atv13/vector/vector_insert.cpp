#include <vector>
#include <iostream>
#include <unistd.h>
#include <omp.h>

double conta_complexa(int i) {
    return 2 * i;
}

int main() {
    int N = 10000; 
    std::vector<double> vec;
    double start_time = omp_get_wtime();
    
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        #pragma omp critical
        {
            vec.push_back(conta_complexa(i));
        }
    }
    
    double end_time = omp_get_wtime();
    std::cout << "Time: " << end_time - start_time << " seconds" << std::endl;

    for (int i = 0; i < N; i++) {
        std::cout << vec[i] << " ";
    }

    std::cout << std::endl;
    return 0;
}
