import numpy as np
import random as rd
from random import randint

def fitness(weight, value, population, capacity):
  fitness = np.empty(len(population))
  for i in range(len(fitness)):
    S1 = np.sum(population[i] * value)
    S2 = np.sum(population[i] * weight)
    if S2 <= capacity:
      fitness[i] = S1
    else:
      fitness[i] = 0
  return fitness.astype(int)

def selection(fitness, num_parents, population):
  fitness = list(fitness)
  parents = np.empty((num_parents, population.shape[1]))
  for i in range(num_parents):
    max_fitness_idx = np.where(fitness == np.max(fitness))
    parents[i,:] = population[max_fitness_idx[0][0], :]
    fitness[max_fitness_idx[0][0]] = -9999
  return parents

def crossover(parents, num_offsprings):
  offsprings = np.empty((num_offsprings, parents.shape[1]))
  crossover_point = int(parents.shape[1]/2)
  crossover_rate = 0.8
  # i representa o indice dos pais
  i = 0
  # cnt_offsprings = numero de filhos gerados 
  cnt_offsprings = 0
  
  while (cnt_offsprings < num_offsprings):
    parent1_index = i%parents.shape[0] #index do genitor 1
    parent2_index = (i+1)%parents.shape[0] #indice do genitor 2
    x = rd.random()
    if x > crossover_rate:
      pass
    else:
      offsprings[cnt_offsprings,0:crossover_point] = parents[parent1_index,0:crossover_point]
      offsprings[cnt_offsprings,crossover_point:] = parents[parent2_index,crossover_point:]
      cnt_offsprings +=1
    i += 1
  return offsprings

def mutation(offsprings):
    mutants = np.empty((offsprings.shape))
    mutation_rate = 0.15
    for i in range(mutants.shape[0]):
        random_value = rd.random()
        mutants[i,:] = offsprings[i,:]
        if random_value > mutation_rate:
            continue
        int_random_value = randint(0,offsprings.shape[1]-1)    
        if mutants[i,int_random_value] == 0 :
            mutants[i,int_random_value] = 1
        else :
            mutants[i,int_random_value] = 0
    return mutants   

def optimize(weight, value, population, pop_size, num_generations, capacity):
    parameters, fitness_history = [], []
    num_parents = int(pop_size[0]/2)
    num_offsprings = pop_size[0] - num_parents 
    for i in range(num_generations):
        fitness_ = fitness(weight, value, population, capacity)
        fitness_history.append(fitness_)
        parents = selection(fitness_, num_parents, population)
        offsprings = crossover(parents, num_offsprings)
        mutants = mutation(offsprings)
        population[0:parents.shape[0], :] = parents
        population[parents.shape[0]:, :] = mutants
        
    # print('Última geraçào: \n{}\n'.format(population)) 
    fitness_last_gen = fitness(weight, value, population, capacity)      
    # print('Fitness da última geração: \n{}\n'.format(fitness_last_gen))
    max_fitness = np.where(fitness_last_gen == np.max(fitness_last_gen))
    parameters.append(population[max_fitness[0][0],:])
    return parameters, fitness_history

if __name__ == "__main__":
    item_number = np.arange(1,41) #items que podem ir para a mochila
    weight = np.random.randint(1, 15, size=40) # pesos aleatorios
    value = np.random.randint(1, 100, size=40) # valores aleatorios
    knapsack_capacity = 100 # peso máximo da mochila

    solutions_per_pop = 16
    pop_size = (solutions_per_pop, len(item_number))
    # print(f'Tamanho da população = {pop_size}')
    initial_population = np.random.randint(2, size = pop_size)
    initial_population = initial_population.astype(int)
    num_generations = 1000
    # print(f'População inicial:\n {initial_population}')

    parameters, fitness_history = optimize(weight, value, initial_population, pop_size, num_generations, knapsack_capacity)

    # print('Indivíduo da última geração com maior fitness: \n{}'.format(parameters))

    selected_items = item_number * parameters


    print('\nQuais itens irão na mochila:')
    peso = 0
    for i in range(selected_items.shape[1]):
    
        if selected_items[0][i] != 0:
            print('{}'.format(selected_items[0][i]), end='\t')
            peso = peso + weight[i]


    print('\nPeso total da mochila = ', peso)