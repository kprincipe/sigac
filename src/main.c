#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"
#include "curso.h"
#include "discente.h"
#include "turma.h"

#define UTILS_IMPLEMENTATION
#include "utils.h"

int main(void) {
    // menu em que usuario se encontra
    Menus menu_atual = PRINCIPAL;

    // entrada de opcao  de menu pelo usuario
    char opcao[TAM_MAX];
    
    // vetores com dados a serem populados
    Discente discentes[TAM_MAX];
    Curso cursos[TAM_MAX];
    Turma turmas[TAM_MAX];

    // contadores dos vetores de dados
    int qtd_discentes = 0;
    int qtd_cursos = 0;
    int qtd_turmas = 0;

    // nomes de arquivos onde dados serao armazenados
    char *arquivos[] = {
        "cursos.csv",
        "discentes.csv",
        "turmas.csv"
    };

    // loop de menu
    for (;;) {
        LIMPAR_TELA();
        CABECALHO();
        switch (menu_atual) {
            case PRINCIPAL:
                printf("----: menu principal :----\n");
                printf("(1) cursos\n");
                printf("(2) discentes\n");
                printf("(3) turmas\n\n");
                printf("(4) relatorios\n\n");
                printf("(5) sair\n");
                printf("> ");
                
                // leitura de entrada do usuario
                if (fgets(opcao, TAM_MAX, stdin) == NULL) {
                    menu_atual = INVALIDO;
                }
                
                // efeito colateral da opcao selecionada
                if (*opcao == '1') {
                    menu_atual = CURSOS;
                } else if (*opcao == '2') {
                    menu_atual = DISCENTES;
                } else if (*opcao == '3') {
                    menu_atual = TURMAS;
                } else if (*opcao == '4') {
                    menu_atual = RELATORIOS;
                } else if (*opcao == '5') {
                    menu_atual = SAIR;
                }

                break;
            case CURSOS:
                printf("----: cursos :----\n\n");

                qtd_cursos = popular_cursos(cursos, arquivos[ARQ_CURSOS]);
                mostrar_cursos(cursos, qtd_cursos);
                
                printf("\n------------------\n\n");
                printf("(1) cadastrar curso\n");
                printf("(2) remover curso\n");
                printf("(3) editar curso\n");
                printf("(5) voltar\n\n");
                printf("> ");

                fgets(opcao, TAM_MAX, stdin);
                
                if (*opcao == '1') {
                    LIMPAR_TELA();
                    CABECALHO();
                    printf("----: cadastro de curso :----\n\n");
                    cadastrar_curso(arquivos[ARQ_CURSOS]);
                } else if (*opcao == '2') {
                    LIMPAR_TELA();
                    CABECALHO();
                    printf("----: remover curso :----\n\n");
                    remover_curso(arquivos[ARQ_CURSOS]);
                } else if(*opcao == '3') {
                    LIMPAR_TELA();
                    CABECALHO();
                    printf("----: editar curso :----\n\n");

                    for (int i = 0; i < qtd_cursos; ++i) {
                        printf("%d. %s\n", i, cursos[i].nome);
                    }

                    editar_curso(arquivos[ARQ_CURSOS]);
                } else if(*opcao == '5') {
                    menu_atual = PRINCIPAL;
                } else {
                    LIMPAR_TELA();
                    CABECALHO();
                    printf("opcao invalida\n");
                    getchar();
                }

                break;
            case DISCENTES:
                printf("----: discentes :----\n\n");

                qtd_discentes = popular_discentes(discentes, arquivos[ARQ_DISCENTES]);
                mostrar_discentes(discentes, qtd_discentes);
                
                printf("\n---------------------\n\n");
                printf("(1) cadastrar discente\n");
                printf("(2) remover discente\n");
                printf("(3) editar discente\n");
                printf("(5) voltar\n\n");
                printf("> ");

                fgets(opcao, TAM_MAX, stdin);
                
                if (*opcao == '1') {
                    LIMPAR_TELA();
                    CABECALHO();
                    printf("----: cadastro de discente :----\n\n");
                    cadastrar_discente(arquivos[ARQ_DISCENTES]);
                } else if (*opcao == '2') {
                    LIMPAR_TELA();
                    CABECALHO();
                    printf("----: remover discente :----\n\n");
                    remover_discente(arquivos[ARQ_DISCENTES]);
                } else if(*opcao == '3') {
                    LIMPAR_TELA();
                    CABECALHO();
                    printf("----: discentes :----\n\n");

                    for (int i = 0; i < qtd_discentes; ++i) {
                        printf("%d. %s\n", i, discentes[i].nome);
                    }

                    editar_discente(arquivos[ARQ_DISCENTES]);
                } else if(*opcao == '5') {
                    menu_atual = PRINCIPAL;
                } else {
                    LIMPAR_TELA();
                    CABECALHO();
                    printf("opcao invalida\n");
                    getchar();
                }
                break;
            case TURMAS:
                printf("----: turmas :----\n\n");

                qtd_turmas = popular_turmas(turmas, arquivos[ARQ_TURMAS]);
                mostrar_turmas(turmas, qtd_turmas);

                printf("\n---------------------\n\n");
                printf("(1) cadastrar turma\n");
                printf("(2) remover turma\n");
                printf("(3) editar turma\n");
                printf("(5) voltar\n\n");
                printf("> ");

                fgets(opcao, TAM_MAX, stdin);

                if (*opcao == '1') {
                    LIMPAR_TELA();
                    CABECALHO();
                    printf("----: cadastro de turma :----\n\n");
                    cadastrar_turma(arquivos);
                } else if (*opcao == '2') {
                    LIMPAR_TELA();
                    CABECALHO();
                    printf("----: remover turma :----\n\n");
                    remover_turma(arquivos[ARQ_TURMAS]);
                } else if(*opcao == '3') {
                    LIMPAR_TELA();
                    CABECALHO();
                    printf("----: editar turma :----\n\n");
                    editar_turma(arquivos);
                } else if(*opcao == '5') {
                    menu_atual = PRINCIPAL;
                } else {
                    LIMPAR_TELA();
                    CABECALHO();
                    printf("opcao invalida\n");
                    getchar();
                }
                break;
            case RELATORIOS:
                printf("----: relatorios :----\n\n");

                printf("(1) listagem geral de discentes\n");
                printf("(2) pesquisar discente\n");
                printf("(3) listagem dos cursos\n");
                printf("(4) listagem de discentes em turmas\n");
                printf("(5) listagem de discentes em turmas e curso\n");
                printf("(6) listagem de discentes conforme numero de turma\n");
                printf("(7) listagem turmas com media\n");
                printf("\n(0) voltar\n\n");
                printf("> ");

                fgets(opcao, TAM_MAX, stdin);

                if (*opcao == '1') {
                    LIMPAR_TELA();
                    CABECALHO();
                    printf("----: relatorio discentes :----\n\n");

                    qtd_discentes = popular_discentes(discentes, arquivos[ARQ_DISCENTES]);
                    printf("qtd_discentes: %d\n\n", qtd_discentes);
                    for (int i = 0; i < qtd_discentes; ++i) {
                        printf("+ %s\n", discentes[i].nome);
                        printf("\t  cpf: %s\n", discentes[i].cpf);
                        printf("\tidade: %d\n", discentes[i].idade);
                        printf("\n");
                    }
                    getchar();
                } else if (*opcao == '2') {
                    LIMPAR_TELA();
                    CABECALHO();
                    printf("----: pesquisar discente :----\n\n");
                    pesquisar_discente(arquivos[ARQ_DISCENTES]);
                    getchar();
                } else if (*opcao == '3') {
                    LIMPAR_TELA();
                    CABECALHO();
                    printf("----: relatorio de cursos :----\n\n");

                    qtd_cursos = popular_cursos(cursos, arquivos[ARQ_CURSOS]);
                    printf("quantidade de cursos: %d\n\n", qtd_cursos);
                    for (int i = 0; i < qtd_cursos; ++i) {
                        printf("+ %s\n", cursos[i].nome);
                        printf("\t       codigo: %d\n", cursos[i].codigo);
                        printf("\t        horas: %d\n", cursos[i].horas);
                        printf("\t        vagas: %d\n", cursos[i].numero_vagas);
                        printf("\n");
                    }
                    getchar();
                } else if (*opcao == '4') {
                    // Listagem do número da turma, cpf, nome e nota do discente.
                    LIMPAR_TELA();
                    CABECALHO();

                    qtd_turmas = popular_turmas(turmas, arquivos[ARQ_TURMAS]);
                    qtd_discentes = popular_discentes(discentes, arquivos[ARQ_DISCENTES]);

                    for (int i = 0; i < qtd_discentes; ++i) {
                        for (int j = 0; j < qtd_turmas; ++j) {
                            if (strcmp(discentes[i].cpf, turmas[j].cpf) == 0) {
                                printf("+ %s\n", discentes[i].nome);
                                printf("\tnumero de turma: %d\n", turmas[j].numero);
                                printf("\t            cpf: %s\n", turmas[j].cpf);
                                printf("\t           nota: %.2f\n", turmas[j].nota);
                                printf("\n");
                            }
                        }
                    }

                    getchar();
                } else if (*opcao == '5') {
                    // Listagem dos números das turmas, cpf, nome e nota do discente, bem como o código e nome do curso.
                    LIMPAR_TELA();
                    CABECALHO();

                    qtd_turmas = popular_turmas(turmas, arquivos[ARQ_TURMAS]);
                    qtd_discentes = popular_discentes(discentes, arquivos[ARQ_DISCENTES]);
                    qtd_cursos = popular_cursos(cursos, arquivos[ARQ_CURSOS]);

                    for (int i = 0; i < qtd_discentes; ++i) {
                        for (int j = 0; j < qtd_turmas; ++j) {
                            if (strcmp(discentes[i].cpf, turmas[j].cpf) == 0) {
                                printf("+ %s\n", discentes[i].nome);
                                printf("\tnumero de turma: %d\n", turmas[j].numero);
                                printf("\t            cpf: %s\n", turmas[j].cpf);
                                printf("\t           nota: %.2f\n", turmas[j].nota);

                                int encontrado = 0;
                                for (int k = 0; k < qtd_cursos; ++k) {
                                    if (turmas[j].codigo_curso == cursos[k].codigo) {
                                        printf("\t  nome do curso: %s\n", cursos[k].nome);
                                        printf("\tcodigo do curso: %d\n", cursos[k].codigo);
                                        encontrado = 1;
                                        break;
                                    }
                                }
                                if (!encontrado) {
                                    printf("\t  nome do curso: nao cadastrado\n");
                                    printf("\tcodigo do curso: s/n\n");
                                }
                                printf("\n");
                            }
                        }
                    }

                    getchar();
                } else if (*opcao == '6') {
                    // Listagem do cpf, nome e nota do discente conforme o número da turma.
                    LIMPAR_TELA();
                    CABECALHO();

                    qtd_turmas = popular_turmas(turmas, arquivos[ARQ_TURMAS]);
                    qtd_discentes = popular_discentes(discentes, arquivos[ARQ_DISCENTES]);

                    int nums[TAM_MAX];
                    int qtd_nums = 0, denom = 0;

                    // TODO: achar maneira melhor de fazer isso, nao me parece eficiente
                    // cataloga as turmas existentes
                    for (int i = 0; i < qtd_turmas; ++i) {
                        int repete = 0;
                        for (int j = 0; j < qtd_nums; ++j)
                            if (turmas[i].numero == nums[j]) repete = 1;
                        
                        if (repete) continue;
                        nums[qtd_nums++] = turmas[i].numero;
                    }

                    for (int k = 0; k < qtd_nums; ++k) {
                        printf("+ turma %d\n", nums[k]);
                        for (int m = 0; m < qtd_turmas; ++m) {
                            if (nums[k] == turmas[m].numero) {
                                printf("\t- cpf: %s\n", turmas[m].cpf);
                                printf("\t\t| nota: %.2f\n\n", turmas[m].nota);
                            }
                        }
                    }

                    getchar();
                } else if (*opcao == '7') {
                    // Listagem de todas as turmas, bem como a média das notas dos discentes.
                    
                    LIMPAR_TELA();
                    CABECALHO();

                    qtd_turmas = popular_turmas(turmas, arquivos[ARQ_TURMAS]);

                    int nums[TAM_MAX];
                    int qtd_nums = 0, denom = 0;
                    float media = 0;

                    // TODO: achar maneira melhor de fazer isso, nao me parece eficiente
                    // cataloga as turmas existentes
                    for (int i = 0; i < qtd_turmas; ++i) {
                        int repete = 0;
                        for (int j = 0; j < qtd_nums; ++j)
                            if (turmas[i].numero == nums[j]) repete = 1;
                        
                        if (repete) continue;
                        nums[qtd_nums++] = turmas[i].numero;
                    }

                    for (int k = 0; k < qtd_nums; ++k) {
                        for (int m = 0; m < qtd_turmas; ++m) {
                            if (nums[k] == turmas[m].numero) {
                                media += turmas[m].nota;
                                denom++;
                            }
                        }

                        media /= denom;

                        printf("+ turma %d\n", nums[k]);
                        printf("\tmedia da turma: %.2f\n\n", media);
                    }

                    getchar();
                } else if (*opcao == '0') {
                    menu_atual = PRINCIPAL;
                } else {
                    LIMPAR_TELA();
                    CABECALHO();
                    printf("opcao invalida\n");
                    getchar();
                }
                break;
            case SAIR:
                printf("saindo\n");
                getchar();
                exit(1);
                break;
            case INVALIDO:
            default:
                printf("opção inválida\n");
                getchar();
                menu_atual = PRINCIPAL;
        }
    }
    return 0;
}
