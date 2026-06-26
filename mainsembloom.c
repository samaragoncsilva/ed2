#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "hash.h"
#include "bloom.h"

// Variáveis para as estatísticas
int consultas_realizadas = 0;
int consultas_evitadas_bloom = 0; // Ficará sempre 0 neste arquivo
int falsos_positivos = 0;         // Ficará sempre 0 neste arquivo
double tempo_total_consultas = 0.0;

HashTable* ht;
BloomFilter* bf;

void remover_nova_linha(char* str) {
    str[strcspn(str, "\r\n")] = 0;
}

void cadastrar_usuario(const char* id) {
    inserir_bloom(bf, id);
    inserir_hash(ht, id);
    printf("Registro inserido com sucesso: %s\n", id);
}

// RF02 - CONSULTA SEM BLOOM (Versão para Experimento)
void consultar_usuario(const char* id) {
    consultas_realizadas++;

    // Inicia o cronômetro para medir a busca DIRETA na Hash
    clock_t inicio = clock();

    // PULA O BLOOM E VAI DIRETO NA HASH
    bool hash_indica = buscar_hash(ht, id);

    clock_t fim = clock();
    tempo_total_consultas += ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    if (hash_indica) {
        printf("Usuario encontrado\n");
    } else {
        printf("-> Usuario inexistente\n");
    }
}

void carregar_lote() {
    char nome_arquivo[100];
    printf("Informe o nome do arquivo de texto: ");
    scanf("%s", nome_arquivo);

    FILE* fp = fopen(nome_arquivo, "r");
    if (!fp) {
        printf("Erro ao abrir arquivo.\n");
        return;
    }

    char linha[50];
    int cont = 0;
    while (fgets(linha, sizeof(linha), fp)) {
        remover_nova_linha(linha);
        if (strlen(linha) > 0) {
            inserir_bloom(bf, linha);
            inserir_hash(ht, linha);
            cont++;
        }
    }
    fclose(fp);
    printf("Processamento concluido: %d registros carregados.\n", cont);
}

void exibir_estatisticas() {
    printf("\n=== ESTATÍSTICAS (EXPERIMENTO SEM BLOOM) ===\n");
    printf("Quantidade de elementos armazenados: %d\n", ht->count);
    printf("Quantidade de consultas realizadas: %d\n", consultas_realizadas);
    printf("Tempo medio de consulta: %.8f segundos\n", (consultas_realizadas > 0) ? (tempo_total_consultas / consultas_realizadas) : 0.0);
    printf("Quantidade de colisoes na Tabela Hash: %d\n", ht->colisoes);
    printf("==========================================\n");
}

int main() {
    ht = criar_hash(200003);
    bf = criar_bloom(1000000, 7);

    int opcao;
    char id[50];

    do {
        printf("\n=== MENU SEM BLOOM ===\n");
        printf("1. Inserçao\n2. Consulta\n3. Estatisticas\n4. Inserir em lote\n0. Sair\nSelecione: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: printf("ID: "); scanf("%s", id); cadastrar_usuario(id); break;
            case 2: printf("ID: "); scanf("%s", id); consultar_usuario(id); break;
            case 3: exibir_estatisticas(); break;
            case 4: carregar_lote(); break;
        }
    } while (opcao != 0);

    liberar_hash(ht);
    liberar_bloom(bf);
    return 0;
}
