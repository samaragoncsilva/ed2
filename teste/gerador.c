#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <direct.h> // Biblioteca para descobrir a pasta atual no Windows

void criar_arquivo_teste(const char* nome_arq, int qtd) {
    FILE* f = fopen(nome_arq, "w");
    if (!f) {
        printf("Erro ao criar o arquivo: %s\n", nome_arq);
        return;
    }

    char letras[] = "abcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < qtd; i++) {
        char nome[12];
        for (int j = 0; j < 8; j++) {
            nome[j] = letras[rand() % 26];
        }
        sprintf(&nome[8], "%03d", rand() % 1000);
        fprintf(f, "%s\n", nome);
    }
    fclose(f);
    printf("Gerado: %s\n", nome_arq);
}

int main() {
    srand(time(NULL));

    // Descobre a pasta exata onde o programa esta rodando
    char pasta_atual[500];
    if (_getcwd(pasta_atual, sizeof(pasta_atual)) != NULL) {
        printf("==================================================\n");
        printf("OS ARQUIVOS ESTAO SENDO SALVOS NESTA PASTA:\n");
        printf("%s\n", pasta_atual);
        printf("==================================================\n\n", pasta_atual);
    }

    printf("Criando os arquivos de teste...\n");

    criar_arquivo_teste("banco_1000.txt", 1000);
    criar_arquivo_teste("busca_1000.txt", 1000);
    criar_arquivo_teste("banco_10000.txt", 10000);
    criar_arquivo_teste("busca_10000.txt", 10000);
    criar_arquivo_teste("banco_100000.txt", 100000);
    criar_arquivo_teste("busca_100000.txt", 100000);

    printf("\nPronto! Va ate a pasta mostrada acima para pegar os arquivos.\n");
    return 0;
}
