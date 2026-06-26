#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "hash.h"
#include "bloom.h"

// Variaveis globais para estatisticas gerais do menu (RF03)
int q_consultas = 0;
int q_evitadas = 0;
int q_falsos_pos = 0;
int q_colisoes_menu = 0; // Conta colisoes das insercoes normais do menu (Opcao 4)
double tempo_total = 0.0;

TabelaHash* th = NULL;
FiltroBloom* fb = NULL;

// RF04 - Leitura simples de arquivo de texto (Lendo de dentro da pasta data/)
void carregar_dados(const char* arquivo) {
    char caminho_completo[150];
    char nome[50];
    int colocados = 0;
    FILE* f = NULL;

    // Tenta abrir exatamente como esta na pasta (Ex: data/banco_1000)
    sprintf(caminho_completo, "data/%s", arquivo);
    f = fopen(caminho_completo, "r");

    // Se nao abrir, tenta colocando o .txt no final por garantia
    if (f == NULL) {
        sprintf(caminho_completo, "data/%s.txt", arquivo);
        f = fopen(caminho_completo, "r");
    }

    if (f == NULL) {
        printf("Erro: Nao achei o arquivo '%s' dentro da pasta data/!\n", arquivo);
        return;
    }

    while (fscanf(f, "%s", nome) != EOF) {
        if (inserir_hash(th, nome)) {
            inserir_bloom(fb, nome);
            colocados++;
        } else {
            q_colisoes_menu++; // Conta colisao/duplicado no carregamento do menu
        }
    }
    fclose(f);
    printf("Sucesso: %d cadastrados via lote.\n", colocados);
}

// Parte 3 - Teste de Desempenho usando os arquivos da pasta data/
void rodar_teste_desempenho() {
    char arq_banco[100], arq_busca[100];
    char caminho_banco[150], caminho_busca[150];
    int n;
    int falsos_p = 0;
    int colisoes_tst = 0; // Nova variavel para contar colisoes EXCLUSIVAS deste teste
    char user[50];
    clock_t i_sem, f_sem, i_com, f_com;
    double t_sem, t_com;
    FILE *f1, *f2, *f3;
    TabelaHash* th_tst = NULL;
    FiltroBloom* fb_tst = NULL;

    printf("\n--- CONFIGURAR EXPERIMENTOS DA PARTE 3 ---\n");
    printf("Digite a quantidade do cenario (1000, 10000 ou 100000): ");
    scanf("%d", &n);

    printf("Digite o nome exato do arquivo do banco (Ex: banco_1000): ");
    scanf("%s", arq_banco);
    printf("Digite o nome exato do arquivo de busca (Ex: busca_1000): ");
    scanf("%s", arq_busca);

    // Monta a primeira tentativa de caminho com o nome seco
    sprintf(caminho_banco, "data/%s", arq_banco);
    sprintf(caminho_busca, "data/%s", arq_busca);

    // Cria as estruturas temporarias exclusivas para este teste isolado
    th_tst = criar_tabela(n * 2);
    fb_tst = criar_bloom();

    // 1. Abre o arquivo do banco para povoar as duas estruturas
    f1 = fopen(caminho_banco, "r");
    if (f1 == NULL) { // Tenta com .txt se falhar
        sprintf(caminho_banco, "data/%s.txt", arq_banco);
        f1 = fopen(caminho_banco, "r");
    }
    if(!f1) {
        printf("Erro: Nao abriu o arquivo do banco em '%s'.\n", caminho_banco);
        liberar_hash(th_tst); liberar_bloom(fb_tst);
        return;
    }

    while(fscanf(f1, "%s", user) != EOF) {
        if (inserir_hash(th_tst, user)) {
            inserir_bloom(fb_tst, user);
        } else {
            colisoes_tst++; // Rastreia as colisoes geradas pelo arquivo do teste!
        }
    }
    fclose(f1);

    // 2. Abre o arquivo de busca para medir o tempo SEM usar o Bloom
    f2 = fopen(caminho_busca, "r");
    if (f2 == NULL) {
        sprintf(caminho_busca, "data/%s.txt", arq_busca);
        f2 = fopen(caminho_busca, "r");
    }
    if(!f2) {
        printf("Erro: Nao abriu o arquivo de busca em '%s'.\n", caminho_busca);
        liberar_hash(th_tst); liberar_bloom(fb_tst);
        return;
    }

    i_sem = clock();
    while(fscanf(f2, "%s", user) != EOF) {
        buscar_hash(th_tst, user); // vai direto na tabela hash
    }
    f_sem = clock();
    t_sem = (double)(f_sem - i_sem) / CLOCKS_PER_SEC;
    fclose(f2);

    // 3. Abre o arquivo de busca de novo para medir o tempo COM o Bloom ativo
    f3 = fopen(caminho_busca, "r");
    if (f3 == NULL) {
        sprintf(caminho_busca, "data/%s.txt", arq_busca);
        f3 = fopen(caminho_busca, "r");
    }
    if(!f3) {
        printf("Erro: Nao abriu o arquivo de busca em '%s'.\n", caminho_busca);
        liberar_hash(th_tst); liberar_bloom(fb_tst);
        return;
    }

    i_com = clock();
    while(fscanf(f3, "%s", user) != EOF) {
        // Fluxo obrigatorio: consulta primeiro o bloom
        if (checar_bloom(fb_tst, user)) {
            // Se o bloom der positivo, confirma na hash
            if (!buscar_hash(th_tst, user)) {
                falsos_p++; // Se nao estava na hash, e um falso positivo
            }
        }
    }
    f_com = clock();
    t_com = (double)(f_com - i_com) / CLOCKS_PER_SEC;
    fclose(f3);

    // Exibe a tabela final INCLUINDO AS COLISOES para você copiar para o seu relatorio!
    printf("\n=================================================================================\n");
    printf("Quantidade\tColisoes Hash\tTempo sem Bloom \tTempo com Bloom \tFalsos Positivos\n");
    printf("---------------------------------------------------------------------------------\n");
    printf("%d\t\t%d\t\t%.6f s\t\t%.6f s\t\t%.2f%%\n", n, colisoes_tst, t_sem, t_com, ((double)falsos_p / n) * 100.0);
    printf("=================================================================================\n");

    // Limpa a memoria do teste da rodada
    liberar_hash(th_tst);
    liberar_bloom(fb_tst);
}

int main() {
    int opt;
    char username[50];
    char nome_arq[100];
    clock_t c_ini, c_fim;
    int na_bloom, achou_final;
    double tx, med;

    // Aloca as estruturas principais que vao rodar no menu interativo
    th = criar_tabela(250000);
    fb = criar_bloom();

    while (1) {
        printf("\n>>> MENU PRINCIPAL <<<\n");
        printf("1 - [RF01] Inserir Usuario\n");
        printf("2 - [RF02] Consultar Usuario\n");
        printf("3 - [RF03] Exibir Estatisticas\n");
        printf("4 - [RF04] Inserir em Lote (Arquivo)\n");
        printf("5 - [Parte 3] Executar Testes de Tempo\n");
        printf("6 - Sair\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opt) != 1) break;

        switch (opt) {
            case 1:
                printf("Digite o identificador (Ex: joao123): ");
                scanf("%s", username);
                if (inserir_hash(th, username)) {
                    inserir_bloom(fb, username);
                    printf("INSERIR %s -> OK\n", username);
                } else {
                    printf("Erro: Usuario ja cadastrado.\n");
                    q_colisoes_menu++;
                }
                break;

            case 2:
                printf("CONSULTAR ");
                scanf("%s", username);

                c_ini = clock();
                q_consultas++;

                // Fluxo obrigatorio determinado pelo enunciado
                na_bloom = checar_bloom(fb, username);
                achou_final = 0;

                if (na_bloom == 0) {
                    q_evitadas++;
                    achou_final = 0;
                } else {
                    achou_final = buscar_hash(th, username);
                    if (achou_final == 0) {
                        q_falsos_pos++;
                    }
                }
                c_fim = clock();
                tempo_total += (double)(c_fim - c_ini) / CLOCKS_PER_SEC;

                if (achou_final) {
                    printf("-> Usuario encontrado\n");
                } else {
                    printf("-> Usuario inexistente\n");
                }
                break;

            case 3:
                printf("\n--- METRICAS DO SISTEMA (RF03) ---\n");
                printf("Quantidade de elementos armazenados: %d\n", th->qtd_elementos);
                printf("Quantidade de colisoes na Tabela Hash: %d\n", q_colisoes_menu);
                printf("Quantidade de consultas realizadas: %d\n", q_consultas);
                printf("Consultas evitadas pela Bloom: %d\n", q_evitadas);
                printf("Numero de falsos positivos: %d\n", q_falsos_pos);

                tx = 0.0;
                if (q_consultas > 0) {
                    tx = ((double)q_falsos_pos / q_consultas) * 100.0;
                }
                printf("Taxa percentual de falsos positivos: %.2f%%\n", tx);

                med = 0.0;
                if (q_consultas > 0) {
                    med = (tempo_total / q_consultas) * 1000.0;
                }
                printf("Tempo medio de consulta: %.6f ms\n", med);
                break;

            case 4:
                printf("Digite o nome do arquivo dentro da pasta data: ");
                scanf("%s", nome_arq);
                carregar_dados(nome_arq);
                break;

            case 5:
                rodar_teste_desempenho();
                break;

            case 6:
                liberar_hash(th);
                liberar_bloom(fb);
                printf("Saindo do programa.\n");
                return 0;

            default:
                printf("Opcao invalida!\n");
        }
    }
    return 0;
}
