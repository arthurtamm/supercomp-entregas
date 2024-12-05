#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <numeric>

using namespace std;

struct Individual {
    vector<int> chromosome;
    int fitness;
};

int calculate_fitness(const vector<int>& weight, const vector<int>& value, const vector<int>& chromosome, int capacity) {
    int total_value = 0, total_weight = 0;
    for (size_t i = 0; i < chromosome.size(); ++i) {
        if (chromosome[i] == 1) {
            total_value += value[i];
            total_weight += weight[i];
        }
    }
    return total_weight <= capacity ? total_value : 0;
}

vector<Individual> initialize_population(int population_size, int chromosome_length) {
    vector<Individual> population(population_size);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, 1);

    for (auto& individual : population) {
        individual.chromosome.resize(chromosome_length);
        generate(individual.chromosome.begin(), individual.chromosome.end(), [&]() { return dist(gen); });
        individual.fitness = 0;
    }

    return population;
}

vector<Individual> select_parents(const vector<Individual>& population, int num_parents) {
    vector<Individual> parents;
    vector<Individual> sorted_population = population;

    sort(sorted_population.begin(), sorted_population.end(), [](const Individual& a, const Individual& b) {
        return a.fitness > b.fitness;
    });

    for (int i = 0; i < num_parents; ++i) {
        parents.push_back(sorted_population[i]);
    }

    return parents;
}

vector<Individual> crossover(const vector<Individual>& parents, int num_offsprings) {
    vector<Individual> offsprings(num_offsprings);
    int crossover_point = parents[0].chromosome.size() / 2;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(0, 1);

    for (int i = 0; i < num_offsprings; ++i) {
        int parent1_idx = i % parents.size();
        int parent2_idx = (i + 1) % parents.size();

        offsprings[i].chromosome.resize(parents[0].chromosome.size());
        if (dist(gen) > 0.8) {
            continue;
        }

        copy(parents[parent1_idx].chromosome.begin(), parents[parent1_idx].chromosome.begin() + crossover_point, offsprings[i].chromosome.begin());
        copy(parents[parent2_idx].chromosome.begin() + crossover_point, parents[parent2_idx].chromosome.end(), offsprings[i].chromosome.begin() + crossover_point);
    }

    return offsprings;
}

void mutate(vector<Individual>& offsprings) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> mutation_chance(0, 1);
    uniform_int_distribution<> mutation_position(0, offsprings[0].chromosome.size() - 1);

    for (auto& offspring : offsprings) {
        if (mutation_chance(gen) < 0.15) {
            int pos = mutation_position(gen);
            offspring.chromosome[pos] = 1 - offspring.chromosome[pos];
        }
    }
}

void optimize(vector<int>& weight, vector<int>& value, int capacity, int population_size, int num_generations) {
    int chromosome_length = weight.size();
    auto population = initialize_population(population_size, chromosome_length);
    int num_parents = population_size / 2;
    int num_offsprings = population_size - num_parents;

    for (int generation = 0; generation < num_generations; ++generation) {
        for (auto& individual : population) {
            individual.fitness = calculate_fitness(weight, value, individual.chromosome, capacity);
        }

        auto parents = select_parents(population, num_parents);
        auto offsprings = crossover(parents, num_offsprings);
        mutate(offsprings);

        for (int i = 0; i < num_parents; ++i) {
            population[i] = parents[i];
        }

        for (int i = 0; i < num_offsprings; ++i) {
            population[num_parents + i] = offsprings[i];
        }
    }

    auto best_individual = max_element(population.begin(), population.end(), [](const Individual& a, const Individual& b) {
        return a.fitness < b.fitness;
    });

    cout << "Melhor fitness: " << best_individual->fitness << endl;
    cout << "Itens escolhidos: ";

    int total_weight = 0;
    for (size_t i = 0; i < best_individual->chromosome.size(); ++i) {
        if (best_individual->chromosome[i] == 1) {
            cout << (i + 1) << " ";
            total_weight += weight[i];
        }
    }
    cout << "\nPeso total: " << total_weight << endl;
}

int main() {
    vector<int> weight(40), value(40);
    int capacity = 100;
    int population_size = 16;
    int num_generations = 1000;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> weight_dist(1, 15);
    uniform_int_distribution<> value_dist(1, 100);

    generate(weight.begin(), weight.end(), [&]() { return weight_dist(gen); });
    generate(value.begin(), value.end(), [&]() { return value_dist(gen); });

    optimize(weight, value, capacity, population_size, num_generations);

    return 0;
}
