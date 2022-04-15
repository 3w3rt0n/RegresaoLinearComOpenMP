# Regressão Linear com OpenMP em C++

Existem diferentes métodos para encontrar os coeficientes estimadores de uma
regressão linear, alguns métodos são determinísticos como o do mínimos quadrados outros
métodos são estocásticos como por exemplo o Gradiente Descendente (GD), cada método
com seus pontos positivos e negativos.

O método dos mínimos dos quadrados é a forma de estimação mais amplamente
utilizada na econometria. Consiste em um estimador que minimiza a soma dos quadrados dos
resíduos da regressão, de forma a maximizar o grau de ajuste do modelo aos dados
observados.

Esse é o método que obtém a melhor resposta para os coeficientes da regressão linear
entretanto acaba sendo inviável para problemas muito grandes.

![Regressão Linear](https://github.com/3w3rt0n/RegresaoLinearComOpenMP/blob/main/Hardware/regressaoLinearSimples.png?raw=true)

O método dos mínimos quadrados minimiza a soma dos quadrado dos resíduos, a ideia
por trás dessa técnica é que, minimizando a soma do quadrado dos resíduos, encontraremos α e
β que trarão a menor diferença entre a previsão de γ e o γ realmente observado.

Foram feitas 4 simulações, uma implementação "serial" sem uso de programação paralela e 3 simulações utilizando programação paralela com diferentes quantidades de Thread paralelas.

Resultados obtidos:

![Resultados](https://github.com/3w3rt0n/RegresaoLinearComOpenMP/blob/main/Hardware/resultados.png?raw=true)

É realmente impressionante a diminuição do tempo de execução do algoritmo com o uso de programação paralela.

# Supercomputador - IMD

O supercomputador está localizado no datacenter do IMD.

![IMD](https://github.com/3w3rt0n/RegresaoLinearComOpenMP/blob/main/Hardware/6CVIT.jpg?raw=true)

Imagem do supercomputador.

![SuperComputador](https://github.com/3w3rt0n/RegresaoLinearComOpenMP/blob/main/Hardware/5SUPERCOMPUTADOR.jpg?raw=true)

Configuração do hardware do supercomputador.

![Especificações](https://github.com/3w3rt0n/RegresaoLinearComOpenMP/blob/main/Hardware/hardware.png?raw=true)

