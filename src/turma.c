#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "turma.h"
#include "discente.h"
#include "curso.h"
#include "utils.h"

void cadastrar_turma(char **arquivos) {
    Turma turma = {0};

    char buffer[TAM_MAX];

    Discente discentes[TAM_MAX];
    int qtd_cads = popular_discentes(discentes, arquivos[ARQ_DISCENTES]);

    Curso cursos[TAM_MAX];
    int qtd_cursos = popular_cursos(cursos, arquivos[ARQ_CURSOS]);

    printf("numero: ");
    fgets(buffer, TAM_MAX, stdin);
    cortar_nl(buffer);
    turma.numero = atoi(buffer);

    for (int i = 0; i < qtd_cursos; ++i) {
        printf("\t+ %d - %s\n", cursos[i].codigo, cursos[i].nome);
    }

    printf("\ncodigo do curso: ");
    fgets(buffer, TAM_MAX, stdin);
    cortar_nl(buffer);
    turma.codigo_curso = atoi(buffer);

    printf("ano: ");
    fgets(buffer, TAM_MAX, stdin);
    cortar_nl(buffer);
    turma.ano = atoi(buffer);

    int cpfs[TAM_MAX];
    float notas[TAM_MAX];
    float horas[TAM_MAX];

    int cont = 0;

    for (;;) {
        LIMPAR_TELA();
        CABECALHO();
        printf("----: adicionar discentes :----\n\n");
        for (int i = 0; i < qtd_cads; ++i) {
            int add = 0;

            for (int j = 0; j < cont; ++j) {
                if (i == cpfs[j]) add = 1;
            }

            if (!add) {
                printf("+ (%d) %s (%s)\n", i, discentes[i].nome, discentes[i].cpf);
            }
        }

        printf("\n----: discentes adicionados :----\n\n");

        if (cont == 0) {
            printf("nenhum discente adicionado!!\n");
        } else {
            for (int i = 0; i < cont; ++i) {
                printf("+ %s (%s)\n", discentes[cpfs[i]].nome, discentes[cpfs[i]].cpf);
            }
        }
        
        printf("\n---------------------------------\n\n");
        printf("digite o numero do discente para adicionar ou retirar\n");
        printf("obs: insira 'r' para recomecar, 't' para terminar e 'q' para sair\n");
        printf("> ");
        
        fgets(buffer, TAM_MAX, stdin);
        cortar_nl(buffer);
        
        if (*buffer == 'q') return;
        if (*buffer == 't') break;
        if (*buffer == 'r') {
            cont = 0;
            continue;
        }

        cpfs[cont++] = atoi(buffer);
    }

    for (int i = 0; i < cont; ++i) {
        LIMPAR_TELA();
        CABECALHO();
        printf("----: adicionar discentes :----\n\n");
        
        printf("discente: %s (%s)\n\n", discentes[cpfs[i]].nome, discentes[cpfs[i]].cpf);

        printf("nota: ");
        fgets(buffer, TAM_MAX, stdin);
        cortar_nl(buffer);
        turma.nota = atof(buffer);

        printf("horas de participacao: ");
        fgets(buffer, TAM_MAX, stdin);
        cortar_nl(buffer);
        turma.horas_participacao = atof(buffer);

        strcpy(turma.cpf, discentes[cpfs[i]].cpf);

        salvar_turma(turma, arquivos[ARQ_TURMAS]);
    }
}

int popular_turmas(Turma *turmas, char *arquivo) {
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
        strcpy(turmas[i].cpf, buff);

        extrair_item(f, buff);
        turmas[i].numero = atoi(buff);

        extrair_item(f, buff);
        turmas[i].codigo_curso = atoi(buff);
        
        extrair_item(f, buff);
        turmas[i].ano = atoi(buff);

        extrair_item(f, buff);
        turmas[i].nota = atof(buff);

        extrair_item(f, buff);
        turmas[i].horas_participacao = atof(buff);
    }
}

void mostrar_turmas(Turma *turmas, int qtd_turmas) {
    int turmas_num[TAM_MAX], exibir;
    int qtd_num = 0;

    if (qtd_turmas > 0) {
        for (int i = 0; i < qtd_turmas; ++i) {
            exibir = 1;
            for (int j = 0; j < qtd_num; ++j) {
                if (turmas_num[j] == turmas[i].numero) {
                    exibir = 0;
                    break;
                }
            }

            if (exibir) {
                printf("+ turma %d\n", turmas[i].numero);
                turmas_num[qtd_num++] = turmas[i].numero;
            }
        }
    } else {
        printf("nenhuma turma encontrada\n");
    }
}

void remover_uma_turma(int indice, char *arquivo) {
    Turma turmas[TAM_MAX];
    int qtd_turmas = popular_turmas(turmas, arquivo);
    
    for (int i = 0; i < qtd_turmas; ++i) {
        if (i == indice) continue;
        salvar_turma(turmas[i], "tmp.csv");
    }
    rename("tmp.csv", arquivo);
}

void remover_turma(char *arquivo) {
    Turma turmas[TAM_MAX];
    char buffer[TAM_MAX];

    int qtd_turmas = popular_turmas(turmas, arquivo);
    int nums[TAM_MAX], qtd_nums = 0;

    for (int i = 0; i < qtd_turmas; ++i) {
        int repete = 0;
        for (int j = 0; j < qtd_nums; ++j)
            if (turmas[i].numero == nums[j]) repete = 1;

        if (repete) continue;
        nums[qtd_nums++] = turmas[i].numero;
    }

    if (qtd_nums == 1) {
        remove(arquivo);
        return;
    }

    for (int i = 0; i < qtd_nums; ++i) {
        printf("%d. turma %d\n", i, nums[i]);
    }

    printf("\ninsira o numero da turma para remover\n");
    printf("> ");

    fgets(buffer, TAM_MAX, stdin);
    cortar_nl(buffer);
    
    for (int k = 0; k < qtd_turmas; ++k) {
        if (turmas[k].numero == nums[atoi(buffer)]) continue;
        salvar_turma(turmas[k], "tmp.csv");
    }
    
    rename("tmp.csv", arquivo);
}

void editar_turma(char *arquivos[]) {
    Turma turmas[TAM_MAX];
    Discente discentes[TAM_MAX];
    Curso cursos[TAM_MAX];

    char buffer[TAM_MAX];

    int qtd_turmas = popular_turmas(turmas, arquivos[ARQ_TURMAS]);
    int qtd_cads = popular_discentes(discentes, arquivos[ARQ_DISCENTES]);
    int qtd_cursos = popular_cursos(cursos, arquivos[ARQ_CURSOS]);
    int nums[TAM_MAX], qtd_nums = 0;

    for (int i = 0; i < qtd_turmas; ++i) {
        int repete = 0;
        for (int j = 0; j < qtd_nums; ++j)
            if (turmas[i].numero == nums[j]) repete = 1;

        if (repete) continue;
        nums[qtd_nums++] = turmas[i].numero;
    }

    for (int i = 0; i < qtd_nums; ++i) {
        printf("%d. turma %d\n", i, nums[i]);
    }

    printf("\ninsira o numero da turma para editar\n");
    printf("> ");

    fgets(buffer, TAM_MAX, stdin);
    cortar_nl(buffer);


    for (int k = 0; k < qtd_turmas; ++k) {
        if (turmas[k].numero == nums[atoi(buffer)]) {
            printf("deixe campos em branco para manter inalterado!\n\n");
            printf("editando turma %d\n\n", turmas[k].numero);

            printf("numero (%d): ", turmas[k].numero);
            fgets(buffer, TAM_MAX, stdin);
            if (*buffer != '\n') {
                cortar_nl(buffer);
                turmas[k].numero = atoi(buffer);
            }

            for (int i = 0; i < qtd_cursos; ++i) {
                printf("\t+ %d - %s\n", cursos[i].codigo, cursos[i].nome);
            }

            printf("\ncodigo do curso (%d): ", turmas[k].codigo_curso);
            fgets(buffer, TAM_MAX, stdin);
            if (*buffer != '\n') {
                cortar_nl(buffer);
                turmas[k].codigo_curso = atoi(buffer);
            }

            printf("\nano (%d): ", turmas[k].ano);
            fgets(buffer, TAM_MAX, stdin);
            if (*buffer != '\n') {
                cortar_nl(buffer);
                turmas[k].ano = atoi(buffer);
            }

            int cpfs[TAM_MAX];
            int cont = 0;

            for (;;) {
                LIMPAR_TELA();
                CABECALHO();
                printf("editando turma %d\n\n", turmas[k].numero);
                printf("----: adicionar discentes :----\n\n");
                
                for (int x = 0; x < qtd_cads; ++x) {
                    int add = 0;

                    for (int y = 0; y < cont; ++y)
                        if (x == cpfs[y]) add = 1;

                    if (!add) {
                        printf("+ (%d) %s (%s)\n", x, discentes[x].nome, discentes[x].cpf);
                    }
                }

                printf("\n----: discentes adicionados :----\n\n");

                if (cont == 0) {
                    printf("nenhum discente adicionado!!\n");
                } else {
                    for (int z = 0; z < cont; ++z) {
                        printf("+ %s (%s)\n", discentes[cpfs[z]].nome, discentes[cpfs[z]].cpf);
                    }
                }
                printf("\n---------------------------------\n\n");
                printf("digite o numero do discente para adicionar ou retirar\n");
                printf("obs: insira 'r' para recomecar, 't' para terminar e 'q' para sair\n");
                printf("> ");

                fgets(buffer, TAM_MAX, stdin);
                cortar_nl(buffer);

                if (*buffer == 'q') return;
                if (*buffer == 't') break;
                if (*buffer == 'r') {
                    cont = 0;
                    continue;
                }

                cpfs[cont++] = atoi(buffer);
            }

            for (int h = 0; h < cont; ++h) {
                remover_uma_turma(k, arquivos[ARQ_TURMAS]);
                strcpy(turmas[k].cpf, discentes[cpfs[h]].cpf);
                salvar_turma(turmas[k], arquivos[ARQ_TURMAS]);
            }

            return;
        }
    }
}

void salvar_turma(Turma turma, char *arquivo) {
    FILE *f = fopen(arquivo, "a");

    fprintf(f, "%s,%d,%d,%d,%f,%f\n", turma.cpf, turma.numero, turma.codigo_curso, turma.ano, turma.nota, turma.horas_participacao);

    fclose(f);
}
