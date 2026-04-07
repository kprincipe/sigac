#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "discente.h"
#include "utils.h"

void cadastrar_discente(char *arquivo) {
    Discente discente = {0};

    char buff[TAM_MAX];
    
    printf("nome: ");
    fgets(discente.nome, TAM_MAX, stdin);
    cortar_nl(discente.nome);

    printf("cpf: ");
    fgets(discente.cpf, TAM_MAX, stdin);
    cortar_nl(discente.cpf);

    printf("idade: ");
    fgets(buff, TAM_MAX, stdin);
    cortar_nl(buff);
    discente.idade = atoi(buff);

    salvar_discente(discente, arquivo);
}

int popular_discentes(Discente *discentes, char *arquivo) {
    FILE *f = fopen(arquivo, "r");

    if (f == NULL) return 0;

    int qtd_cads = contar_cadastros(arquivo);
    char buff[TAM_MAX];

    if (qtd_cads == 0) {
        fclose(f);
        return 0;
    }

    for (int i = 0; i < qtd_cads; ++i) {
        extrair_item(f, buff);
        strcpy(discentes[i].nome, buff);

        extrair_item(f, buff);
        strcpy(discentes[i].cpf, buff);
        
        extrair_item(f, buff);
        discentes[i].idade = atoi(buff);
    }

    return qtd_cads;
}

void mostrar_discentes(Discente *discentes, int qtd_discentes) {
    if (qtd_discentes > 0) {
        for (int i = 0; i < qtd_discentes; ++i) {
            printf("+ %s\n", discentes[i].nome);
        }
    } else {
        printf("nenhum discente encontrado\n");
    }
}

void mostrar_discente(Discente discente) {
    printf("+ %s\n", discente.nome);
}

void remover_um_discente(char *arquivo, int i) {
    Discente discentes[TAM_MAX];
    int qtd_discentes = popular_discentes(discentes, arquivo);

    if (qtd_discentes == 1) {
        FILE *f = fopen("tmp.csv", "w");
        fclose(f);
        rename("tmp.csv", arquivo);
        return;
    }

    for (int k = 0; k < qtd_discentes; ++k) {
        if (i == k) continue;
        salvar_discente(discentes[k], "tmp.csv");
    }

    rename("tmp.csv", arquivo);
}

void remover_discente(char *arquivo) {
    Discente discentes[TAM_MAX];
    char buffer[TAM_MAX];

    int qtd_discentes = popular_discentes(discentes, arquivo);

    for (int i = 0; i < qtd_discentes; i++) {
        printf("%d. %s\n", i, discentes[i].nome);
    }

    printf("\ninsira o numero discente para remover\n");
    printf("> ");

    fgets(buffer, TAM_MAX, stdin);
    
    remover_um_discente(arquivo, atoi(buffer));
}

void editar_discente(char *arquivo) {
    Discente discentes[TAM_MAX];
    char buffer[TAM_MAX];

    int qtd_discentes = popular_discentes(discentes, arquivo);
    int indice;

    printf("\ninsira o numero discente para editar\n");
    printf("> ");

    fgets(buffer, TAM_MAX, stdin);
    indice = atoi(buffer);

    LIMPAR_TELA();
    
    for (int k = 0; k < qtd_discentes; ++k) {
        if (k == indice) {
            printf("deixe campos em branco para manter inalterado!\n\n");

            printf("nome (%s): ", discentes[indice].nome);
            fgets(buffer, TAM_MAX, stdin);
            if (*buffer != '\n') {
                cortar_nl(buffer);
                strcpy(discentes[indice].nome, buffer);
            }

            printf("cpf (%s): ", discentes[indice].cpf);
            fgets(buffer, TAM_MAX, stdin);
            if (*buffer != '\n') {
                cortar_nl(buffer);
                strcpy(discentes[indice].cpf, buffer);
            }

            printf("idade (%d): ", discentes[indice].idade);
            fgets(buffer, TAM_MAX, stdin);
            if (*buffer != '\n') discentes[indice].idade = atoi(buffer);

            continue;
        }
        salvar_discente(discentes[k], "tmp.csv");
    }

    salvar_discente(discentes[indice], "tmp.csv");
    rename("tmp.csv", arquivo);
}

void salvar_discente(Discente discente, char *arquivo) {
    FILE *f = fopen(arquivo, "a");

    fprintf(f, "%s,%s,%d\n", discente.nome, discente.cpf, discente.idade);

    fclose(f);
}

void pesquisar_discente(char *arquivo) {
    char busca[TAM_MAX];
    char buffer[TAM_MAX];

    Discente discentes[TAM_MAX];
    int qtd_discentes = popular_discentes(discentes, arquivo);
    int encontrados = 0;

    printf("> ");
    fgets(buffer, TAM_MAX, stdin);
    cortar_nl(buffer);

    LIMPAR_TELA();
    
    CABECALHO();
    printf("----: resultado :----\n\n");
    for (int i = 0; i < qtd_discentes; ++i) {
        strncpy(busca, discentes[i].nome, strlen(buffer));
        if (strcmp(busca, buffer) == 0) {
            encontrados++;
            mostrar_discente(discentes[i]);
        }
    }

    if (encontrados == 0) {
        printf("nenhum discente encontrado\n");
    }
}
