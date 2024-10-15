#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <omp.h>

void print_iterations(const std::string &description,
                      const std::vector<std::vector<int>> &vectors,
                      const int n)
{
    std::vector<std::string> strings(4, std::string());
    for (int i = 0; i != n; i++)
    {
        for (int j = 0; j != 4; j++)
        {
            const auto &vector = vectors[j];
            auto it = std::find(vector.begin(), vector.end(), i);
            if (it != vector.end())
            {
                strings[j] += "*";
            }
            else
            {
                strings[j] += " ";
            }
        }
    }
    std::cout << description << std::endl;
    for (auto &s : strings)
    {
        std::cout << s << "\n";
    }
    std::cout << std::endl;
}

template <typename T>
double schedule(T function,
              const std::string &description,
              const int n)
{
    std::vector<std::vector<int>> vectors(4, std::vector<int>());
    double start_time = omp_get_wtime();
    function(vectors, n);
    double end_time = omp_get_wtime();
    print_iterations(description, vectors, n);
    return end_time - start_time;
}

void scheduleDefault(std::vector<std::vector<int>> &vectors, int n)
{
#pragma omp parallel num_threads(4), shared(vectors, n)
    {
#pragma omp for
        for (int i = 0; i < n; i++)
        {
            vectors[omp_get_thread_num()].push_back(i);
        }
    }
}

void scheduleStatic(std::vector<std::vector<int>> &vectors, int n)
{
#pragma omp parallel num_threads(4), shared(vectors, n)
    {
#pragma omp for schedule(static)
        for (int i = 0; i < n; i++)
        {
            vectors[omp_get_thread_num()].push_back(i);
        }
    }
}

void scheduleDynamic(std::vector<std::vector<int>> &vectors, int n)
{
#pragma omp parallel num_threads(4), shared(vectors, n)
    {
#pragma omp for schedule(dynamic)
        for (int i = 0; i < n; i++)
        {
            vectors[omp_get_thread_num()].push_back(i);
        }
    }
}

void scheduleGuided(std::vector<std::vector<int>> &vectors, int n)
{
#pragma omp parallel num_threads(4), shared(vectors, n)
    {
#pragma omp for schedule(guided)
        for (int i = 0; i < n; i++)
        {
            vectors[omp_get_thread_num()].push_back(i);
        }
    }
}

void scheduleAuto(std::vector<std::vector<int>> &vectors, int n)
{
#pragma omp parallel num_threads(4), shared(vectors, n)
    {
#pragma omp for schedule(auto)
        for (int i = 0; i < n; i++)
        {
            vectors[omp_get_thread_num()].push_back(i);
        }
    }
}

int main()
{
    const int n = 64;  // Número de iterações maior para medir desempenho
    double time_default, time_static, time_dynamic, time_guided, time_auto;

    // Executando cada scheduler três vezes e calculando o tempo médio
    for (int i = 0; i < 3; i++) {
        time_default += schedule(scheduleDefault, "default:               ", n);
        time_static += schedule(scheduleStatic, "schedule(static):      ", n);
        time_dynamic += schedule(scheduleDynamic, "schedule(dynamic):     ", n);
        time_guided += schedule(scheduleGuided, "schedule(guided):      ", n);
        time_auto += schedule(scheduleAuto, "schedule(auto):        ", n);
    }

    // Calculando médias
    time_default /= 3;
    time_static /= 3;
    time_dynamic /= 3;
    time_guided /= 3;
    time_auto /= 3;

    // Exibindo resultados
    std::cout << "Médias de tempo de execução:\n";
    std::cout << "Default:  " << time_default << " segundos\n";
    std::cout << "Static:   " << time_static << " segundos\n";
    std::cout << "Dynamic:  " << time_dynamic << " segundos\n";
    std::cout << "Guided:   " << time_guided << " segundos\n";
    std::cout << "Auto:     " << time_auto << " segundos\n";

    return 0;
}
