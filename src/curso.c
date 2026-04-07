#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "utils.h"

#include "curso.h"

void cadastrar_curso(char *arquivo) {
    Curso curso = {0};

    char buff[TAM_MAX];
    
    printf("nome: ");
    fgets(curso.nome, TAM_MAX, stdin);
    cortar_nl(curso.nome);

    printf("codigo: ");
    fgets(buff, TAM_MAX, stdin);
    cortar_nl(buff);
    curso.codigo = atoi(buff);

    printf("horas: ");
    fgets(buff, TAM_MAX, stdin);
    cortar_nl(buff);
    curso.horas = atoi(buff);

    printf("numero de vagas: ");
    fgets(buff, TAM_MAX, stdin);
    cortar_nl(buff);
    curso.numero_vagas = atoi(buff);

    salvar_curso(curso, arquivo);
}

int popular_cursos(Curso *cursos, char *arquivo) {
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
        strcpy(cursos[i].nome, buff);

        extrair_item(f, buff);
        cursos[i].codigo = atoi(buff);

        extrair_item(f, buff);
        cursos[i].horas = atoi(buff);

        extrair_item(f, buff);
        cursos[i].numero_vagas = atoi(buff);

        extrair_item(f, buff);
        cursos[i].numero_participantes = atoi(buff);
    }  
}

void mostrar_cursos(Curso *cursos, int qtd_cursos) {
    if (qtd_cursos > 0) {
        for (int i = 0; i < qtd_cursos; ++i) {
            printf("+ %s\n", cursos[i].nome);
        }
    } else {
        printf("nenhum curso encontrado\n");
    }
}

void remover_um_curso(char *arquivo, int i) {
    Curso cursos[TAM_MAX];
    int qtd_cursos = popular_cursos(cursos, arquivo);

    if (qtd_cursos == 1) {
        FILE *f = fopen("tmp.csv", "w");
        fclose(f);
        rename("tmp.csv", arquivo);
        return;
    }

    for (int k = 0; k < qtd_cursos; ++k) {
        if (i == k) continue;
        salvar_curso(cursos[k], "tmp.csv");
    }

    rename("tmp.csv", arquivo);
}

void remover_curso(char *arquivo) {
    Curso cursos[TAM_MAX];
    char buffer[TAM_MAX];

    int qtd_cursos = popular_cursos(cursos, arquivo);

    mostrar_cursos(cursos, qtd_cursos);

    printf("\ninsira o numero curso para remover\n");
    printf("> ");

    fgets(buffer, TAM_MAX, stdin);
    cortar_nl(buffer);
    
    for (int k = 0; k < qtd_cursos; ++k) {
        if (atoi(buffer) == k) continue;
        salvar_curso(cursos[k], "tmp.csv");
    }

    rename("tmp.csv", arquivo);
}

// TODO: fazer uma interface melhor para edicao individual dos campos
void editar_curso(char *arquivo) {
    Curso cursos[TAM_MAX];
    char buffer[TAM_MAX];

    int qtd_cursos = popular_cursos(cursos, arquivo);
    int indice;

    printf("\ninsira o numero curso para editar\n");
    printf("> ");

    fgets(buffer, TAM_MAX, stdin);
    cortar_nl(buffer);
    indice = atoi(buffer);

    LIMPAR_TELA();
    
    for (int k = 0; k < qtd_cursos; ++k) {
        if (k == indice) {
            printf("deixe campos em branco para manter inalterado!\n\n");

            printf("nome (%s): ", cursos[indice].nome);
            fgets(buffer, TAM_MAX, stdin);
            if (*buffer != '\n') {
                cortar_nl(buffer);
                strcpy(cursos[indice].nome, buffer);
            }

            printf("codigo (%d): ", cursos[indice].codigo);
            fgets(buffer, TAM_MAX, stdin);
            if (*buffer != '\n') cursos[indice].codigo = atoi(buffer);

            printf("horas (%d): ", cursos[indice].horas);
            fgets(buffer, TAM_MAX, stdin);
            if (*buffer != '\n') cursos[indice].horas = atoi(buffer);

            printf("numero de vagas (%d): ", cursos[indice].numero_vagas);
            fgets(buffer, TAM_MAX, stdin);
            if (*buffer != '\n') cursos[indice].numero_vagas = atoi(buffer);

            printf("numero de participantes (%d): ", cursos[indice].numero_participantes);
            fgets(buffer, TAM_MAX, stdin);
            if (*buffer != '\n') cursos[indice].numero_participantes = atoi(buffer);

            continue;
        } else {
            salvar_curso(cursos[k], "tmp.csv");
        }
    }

    salvar_curso(cursos[indice], "tmp.csv");
    rename("tmp.csv", arquivo);
}

void salvar_curso(Curso curso, char *arquivo) {
    FILE *f = fopen(arquivo, "a");

    fprintf(f, "%s,%d,%d,%d,%d\n", curso.nome, curso.codigo, curso.horas, curso.numero_vagas, curso.numero_participantes);

    fclose(f);
}
