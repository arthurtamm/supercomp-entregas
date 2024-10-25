## Discussão de Resultados

Ao comparar os dois métodos de cálculo da variância — o método de duas etapas separadas e o método utilizando fusion kernel — observamos que ambos fornecem o mesmo resultado numérico para a variância, mas há uma diferença significativa em termos de desempenho.

O método em duas etapas separadas primeiro aplica uma transformação para calcular as diferenças ao quadrado e, em seguida, realiza uma redução para somar os resultados. Esse processo, embora simples de entender, exige que os dados sejam movidos e processados em duas etapas distintas, o que resulta em um tempo de execução maior (3.18839e-4 segundos).

Por outro lado, o método utilizando fusion kernel combina a transformação e a redução em uma única operação, eliminando a necessidade de mover dados entre diferentes etapas de processamento. Isso reduz o tempo de execução (2.49542e-4 segundos) e otimiza o uso da GPU, tornando o processo mais eficiente.

Essa diferença, embora pequena no exemplo com 1 milhão de elementos, torna-se mais relevante à medida que o tamanho do conjunto de dados aumenta. O fusion kernel se destaca por minimizar a sobrecarga associada à movimentação de dados, combinando múltiplas operações em um único kernel, o que pode resultar em ganhos de desempenho significativos em cenários de maior escala.