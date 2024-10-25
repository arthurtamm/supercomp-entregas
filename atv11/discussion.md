
## Discussão de Resultados

O uso da GPU para cálculos como a análise de variação de preços de ações demonstrou ser altamente eficiente, especialmente para grandes volumes de dados. A utilização de iteradores dinâmicos, como `thrust::constant_iterator`, e funções de transformação, como `thrust::transform` e `thrust::reduce`, permitiu otimizar o processamento ao evitar o consumo excessivo de memória e reduzir o tempo de execução.

Os iteradores dinâmicos ajudam a otimizar o processamento ao gerar os valores conforme necessário, em vez de pré-alocar grandes vetores na memória, o que economiza espaço e acelera as operações. Já as funções de transformação realizam operações elementares em paralelo diretamente na GPU, eliminando a necessidade de transferir dados repetidamente entre a CPU e a GPU. Ao trabalhar diretamente sobre os dados na GPU, essas funções reduzem a latência de memória e aproveitam o paralelismo massivo para acelerar cálculos como a contagem de variações positivas e o cálculo da média dos retornos.

Essas técnicas reforçam a vantagem do uso de iteradores dinâmicos e funções personalizadas na otimização de tarefas complexas de análise de dados, tornando a GPU uma solução ideal para processamento em larga escala.