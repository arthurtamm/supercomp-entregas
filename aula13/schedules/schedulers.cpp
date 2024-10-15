#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <limits>

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
    const int n = 64;  // Número de iterações
    const int num_execucoes = 3; // Número de execuções para calcular variações
    double tempos_default[num_execucoes], tempos_static[num_execucoes], tempos_dynamic[num_execucoes], tempos_guided[num_execucoes], tempos_auto[num_execucoes];

    // Executando cada scheduler várias vezes
    for (int i = 0; i < num_execucoes; i++) {
        tempos_default[i] = schedule(scheduleDefault, "default:               ", n);
        tempos_static[i] = schedule(scheduleStatic, "schedule(static):      ", n);
        tempos_dynamic[i] = schedule(scheduleDynamic, "schedule(dynamic):     ", n);
        tempos_guided[i] = schedule(scheduleGuided, "schedule(guided):      ", n);
        tempos_auto[i] = schedule(scheduleAuto, "schedule(auto):        ", n);
    }

    // Calculando médias
    double media_default = 0, media_static = 0, media_dynamic = 0, media_guided = 0, media_auto = 0;
    for (int i = 0; i < num_execucoes; i++) {
        media_default += tempos_default[i];
        media_static += tempos_static[i];
        media_dynamic += tempos_dynamic[i];
        media_guided += tempos_guided[i];
        media_auto += tempos_auto[i];
    }
    media_default /= num_execucoes;
    media_static /= num_execucoes;
    media_dynamic /= num_execucoes;
    media_guided /= num_execucoes;
    media_auto /= num_execucoes;

    // Calculando variações
    double var_default = *std::max_element(tempos_default, tempos_default + num_execucoes) - *std::min_element(tempos_default, tempos_default + num_execucoes);
    double var_static = *std::max_element(tempos_static, tempos_static + num_execucoes) - *std::min_element(tempos_static, tempos_static + num_execucoes);
    double var_dynamic = *std::max_element(tempos_dynamic, tempos_dynamic + num_execucoes) - *std::min_element(tempos_dynamic, tempos_dynamic + num_execucoes);
    double var_guided = *std::max_element(tempos_guided, tempos_guided + num_execucoes) - *std::min_element(tempos_guided, tempos_guided + num_execucoes);
    double var_auto = *std::max_element(tempos_auto, tempos_auto + num_execucoes) - *std::min_element(tempos_auto, tempos_auto + num_execucoes);

    // Exibindo resultados
    std::cout << "Médias de tempo de execução:\n";
    std::cout << "Default:  " << media_default << " segundos\n";
    std::cout << "Static:   " << media_static << " segundos\n";
    std::cout << "Dynamic:  " << media_dynamic << " segundos\n";
    std::cout << "Guided:   " << media_guided << " segundos\n";
    std::cout << "Auto:     " << media_auto << " segundos\n";

    std::cout << "\nVariações entre execuções:\n";
    std::cout << "Default:  " << var_default << " segundos\n";
    std::cout << "Static:   " << var_static << " segundos\n";
    std::cout << "Dynamic:  " << var_dynamic << " segundos\n";
    std::cout << "Guided:   " << var_guided << " segundos\n";
    std::cout << "Auto:     " << var_auto << " segundos\n";

    // Identificando o menor tempo médio
    double menor_tempo = std::min({media_default, media_static, media_dynamic, media_guided, media_auto});
    std::string melhor_scheduler;

    if (menor_tempo == media_default) melhor_scheduler = "Default";
    else if (menor_tempo == media_static) melhor_scheduler = "Static";
    else if (menor_tempo == media_dynamic) melhor_scheduler = "Dynamic";
    else if (menor_tempo == media_guided) melhor_scheduler = "Guided";
    else melhor_scheduler = "Auto";

    std::cout << "\nScheduler com menor tempo médio: " << melhor_scheduler << " (" << menor_tempo << " segundos)\n";

    return 0;
}
