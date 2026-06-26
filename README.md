\# Sistema de Consulta Eficiente com Tabela Hash e Filtro de Bloom



Este projeto consiste em um \*\*Sistema de Verificação de Cadastro de Usuários\*\* desenvolvido para otimizar o tempo de busca em grandes volumes de dados. O sistema integra duas estruturas complementares:



\- \*\*Filtro de Bloom\*\*: estrutura probabilística que atua como uma camada inicial de descarte para chaves inexistentes.

\- \*\*Tabela Hash com Encadeamento Externo\*\*: estrutura determinística responsável pelo armazenamento e recuperação exata dos registros.



O software foi desenvolvido integralmente na \*\*linguagem C\*\*, cumprindo a restrição de não utilizar bibliotecas prontas para as estruturas de dados.



\---



\# 1. Estrutura do Projeto



```text

projeto/

├── src/

│   ├── hash.c

│   ├── hash.h

│   ├── bloom.c

│   ├── bloom.h

│   └── main.c

├── data/

│   └── usuarios.txt

└── README.md

```



\### Arquivos



\- \*\*hash.h / hash.c\*\*

&#x20; - Definição e implementação da Tabela Hash.

&#x20; - Algoritmo de espalhamento `djb2`.

&#x20; - Tratamento de colisões por encadeamento externo (listas encadeadas).



\- \*\*bloom.h / bloom.c\*\*

&#x20; - Definição e implementação do Filtro de Bloom.

&#x20; - Manipulação do vetor de bits alocado dinamicamente.

&#x20; - Implementação de \*\*3 funções hash independentes\*\*.



\- \*\*main.c\*\*

&#x20; - Fluxo principal do programa.

&#x20; - Interface de menu interativo.

&#x20; - Leitura de arquivos em lote.

&#x20; - Execução dos testes experimentais.



\---



\# 2. Instruções de Compilação e Execução



A compilação deve ser realizada utilizando o compilador \*\*GCC\*\*.



\### Compilação



```bash

gcc main.c hash.c bloom.c -o sistema

```



\### Execução



```bash

./sistema

```



\---



\# 3. Formato dos Arquivos de Entrada (RF04)



Para a inserção em lote, o sistema aceita arquivos `.txt`, onde cada linha deve conter exatamente um identificador de usuário.



As chaves seguem o padrão:



```text

\[8 caracteres]\[3 números]

```



\### Exemplo (`usuarios.txt`)



```text

islaifda122

djskalsa297

fjkldsaf881

joao123000

maria98011

```



\---



\# 4. Guia de Uso do Menu



\## RF01 – Inserção Individual



Cadastra um novo usuário no sistema.



\### Comando



```text

INSERIR joao123

```



\### Saída Esperada



```text

Usuário joao123 cadastrado com sucesso.

```



\---



\## RF02 – Consulta de Existência



O sistema primeiro verifica o \*\*Filtro de Bloom\*\*.



\- Se retornar \*\*0\*\*, a busca termina imediatamente.

\- Se retornar \*\*1\*\*, é feita a confirmação na \*\*Tabela Hash\*\*.



\### Usuário existente



```text

CONSULTAR joao123

```



Saída:



```text

Usuário encontrado (Verificado via Filtro de Bloom e confirmado na Tabela Hash).

```



\### Usuário inexistente



```text

CONSULTAR ana777

```



Saída:



```text

Usuário inexistente (Bloqueado preventivamente pelo Filtro de Bloom).

```



\---



\## RF03 – Estatísticas do Sistema



Exibe informações sobre o funcionamento do sistema.



\### Exemplo de saída



```text

=====================================================

&#x20;              ESTATÍSTICAS DO SISTEMA

=====================================================

Quantidade de elementos armazenados: 10000

Quantidade de consultas realizadas: 20000

Consultas evitadas pelo Filtro de Bloom: 9913

Número de falsos positivos detectados: 87

Taxa percentual de falsos positivos: 0.87%

Tempo médio de consulta: 0.000100 s

=====================================================

```



\---



\# 5. Decisões de Projeto e Dimensionamento



\## Tabela Hash



A Tabela Hash foi dimensionada dinamicamente com tamanho igual a \*\*2N\*\*, mantendo o fator de carga



```text

α = n / m = 0,5

```



Essa estratégia reduziu completamente as colisões durante os testes e manteve a complexidade média de busca em \*\*O(1)\*\*.



\---



\## Filtro de Bloom



O Filtro de Bloom foi configurado com:



\- \*\*900.000 bits\*\*

\- \*\*3 funções hash independentes (k = 3)\*\*



O tamanho foi definido para controlar a taxa de falsos positivos até o cenário máximo de \*\*100.000 registros\*\*.



\---



\# 6. Resultados Experimentais



| Cenário | Colisões na Hash | Tempo sem Bloom | Tempo com Bloom | Falsos Positivos |

|----------|-----------------:|----------------:|----------------:|-----------------:|

| 1.000 | 0 | 0,000 s | 0,000 s | 0,10% |

| 10.000 | 0 | 0,003 s | 0,002 s | 0,87% |

| 100.000 | 0 | 0,026 s | 0,025 s | 8,97% |



\---



\## Análise dos Resultados



\### Cenário 1.000



O tempo registrado foi de \*\*0,000 s\*\*, pois o volume de dados ficou abaixo da resolução da função `clock()` da biblioteca `time.h`.



O Filtro de Bloom apresentou excelente precisão, com apenas \*\*0,10%\*\* de falsos positivos.



\---



\### Cenário 10.000



Foi o melhor cenário observado.



Resultados:



\- redução de \*\*33,3%\*\* no tempo de busca;

\- tempo caiu de \*\*0,003 s\*\* para \*\*0,002 s\*\*;

\- taxa de falsos positivos de apenas \*\*0,87%\*\*.



\---



\### Cenário 100.000



Nesse cenário ocorreu saturação do vetor de bits.



Como consequência:



\- aumento da taxa de falsos positivos para \*\*8,97%\*\*;

\- mais consultas precisaram ser confirmadas na Tabela Hash;

\- o ganho de desempenho foi reduzido para aproximadamente \*\*3,8%\*\* devido ao overhead das verificações adicionais.

