#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#define limpar_tela() system("cls")
#else
#define limpar_tela() system("clear")
#endif

#define TAM_MAX 256

typedef enum {
    PRINCIPAL = 0,
    DISCENTES,
    CURSOS,
    TURMAS,
    RELATORIOS,
    INVALIDO,
    SAIR
} Menus;

typedef enum {
    ARQ_DISCENTES = 0,
    ARQ_CURSOS,
    ARQ_TURMAS
} Arquivos;

typedef struct {
    char nome[TAM_MAX];
    char cpf[TAM_MAX];
    int idade;
} Discente;

typedef struct {
    int codigo;
    int horas;
    int vagas;
    int numero_participantes;
    char nome[TAM_MAX];
} Curso;

typedef struct {
    int numero;
    int codigo;
    int ano;
    float *notas;
    int horas_participacao;
    int qtd_cpf;
    char **cpf;
} Turma;

Discente *popular_discentes(int qtd_cads, char *onde);
int contar_cadastros(char *onde);

void cortar_nl(char *s) {
    int i = 0;
    while (s[i] != '\0') i++;
    s[i - 1] = '\0';
}

void imprimir_discente(Discente discente) {
    printf("+--------------------------------+\n");
    printf("│ Nome  │ %s\n", discente.nome);
    printf("│ CPF   │ %s\n", discente.cpf);
    printf("│ Idade │ %d\n", discente.idade);
    printf("+--------------------------------+\n\n");
}

void imprimir_curso(Curso curso) {
    printf("+--------------------------------+\n");
    printf("│ Nome     │ %s\n", curso.nome);
    printf("│ Codigo   │ %d\n", curso.codigo);
    printf("│ Horas    │ %d h\n", curso.horas);
    printf("│ Vagas    │ %d\n", curso.vagas);
    printf("│ N° Part. │ %d\n", curso.numero_participantes);
    printf("+--------------------------------+\n\n");
}

void imprimir_turma(Turma turma, char *onde[]) {
    int qtd_cads = contar_cadastros(onde[ARQ_DISCENTES]);
    Discente *discentes = popular_discentes(qtd_cads, onde[ARQ_DISCENTES]);

    printf("+------------------------------------------------+\n");
    printf("│ n° turma       │ %d\n", turma.numero);
    printf("│------------------------------------------------+\n");
    for (int i = 0; i < turma.qtd_cpf; ++i) {
        for (int j = 0; j < qtd_cads; ++j) {
            if (strcmp(turma.cpf[i], discentes[j].cpf) == 0) {
                printf("│ %s │ %s\n", turma.cpf[i], discentes[j].nome);
                printf("│ %*.c │ %.2f\n", (int)strlen(turma.cpf[i]), ' ', turma.notas[i]);
                printf("│ %*.c │\n", (int)strlen(turma.cpf[i]), ' ');
            }
        }
    }
    printf("+------------------------------------------------+\n\n");
}

int contar_cadastros(char *onde) {
    int c = 0;

    FILE *f = fopen(onde, "r");
    if (f == NULL) return 0;

    while (!feof(f)) {
        if(fgetc(f) == '\n') c++;
    }
    fclose(f);
    
    return c;
}

void extrair_item(FILE *f, char *dst) {
    int indice = 0;
    char c;
    for (;;) {
        c = fgetc(f);
        if (feof(f) != 0) return;
        if (c == ',' || c == '\n') {
            dst[indice++] = '\0';
            indice = 0;
            break;
        }
        dst[indice++] = c;
    }
}

Discente *popular_discentes(int qtd_cads, char *onde) {
    Discente *discentes = malloc(sizeof(Discente) * qtd_cads);
    char buff[TAM_MAX];

    FILE *f = fopen(onde, "r");
    if (f == NULL) {
        fprintf(stderr, "popular_discentes(): erro: nao foi possivel abrir arquivo: %s\n", onde);
        exit(1);
    };

    for (int i = 0; i < qtd_cads; ++i) {
        extrair_item(f, buff);
        strcpy(discentes[i].nome, buff);

        extrair_item(f, buff);
        strcpy(discentes[i].cpf, buff);

        extrair_item(f, buff);
        discentes[i].idade = atoi(buff);
    }

    return discentes;
}

void exibir_discentes(char *onde) {
    int qtd_cads = contar_cadastros(onde);
    Discente *discentes = popular_discentes(qtd_cads, onde);

    for (int i = 0; i < qtd_cads; ++i) {
        imprimir_discente(discentes[i]);
    }
}

void salvar_discente(char *onde, Discente discente) {
    FILE *f = fopen(onde, "a");
    fprintf(f, "%s,%s,%d\n", discente.nome, discente.cpf, discente.idade);
    fclose(f);
}

void remover_discente(char *onde) {
    int qtd_cads = contar_cadastros(onde);
    char buff[TAM_MAX];

    Discente *discentes = popular_discentes(qtd_cads, onde);

    printf("+--------------------------------+\n");
    for (int i = 0; i < qtd_cads; ++i) {
        printf("│ %d │ %s\n", i, discentes[i].nome);
    }
    printf("+--------------------------------+\n");

    printf("Digite o numero identificador do discente:\n");
    printf("> ");

    fgets(buff, TAM_MAX, stdin);
    cortar_nl(buff);


    for (int k = 0; k < qtd_cads; ++k) {
        if (strcmp(discentes[k].nome, discentes[atoi(buff)].nome) == 0) continue;
        salvar_discente("tmp.csv", discentes[k]);
    }

    rename("tmp.csv", onde);
}

void cadastrar_discente(char *onde) {
    Discente discente = {0};
    char buff[TAM_MAX];

    printf("Nome: ");
    fgets(discente.nome, TAM_MAX, stdin);
    cortar_nl(discente.nome);

    printf("CPF: ");
    fgets(discente.cpf, TAM_MAX, stdin);
    cortar_nl(discente.cpf);

    printf("Idade: ");
    fgets(buff, TAM_MAX, stdin);
    cortar_nl(buff);
    discente.idade = atoi(buff);

    salvar_discente(onde, discente);
}

void pesquisar_discente(char *onde) {
    char buff[TAM_MAX];
    char busca[TAM_MAX];
    int qtd_cads = contar_cadastros(onde);

    Discente *discentes = popular_discentes(qtd_cads, onde);

    printf("> ");
    fgets(busca, TAM_MAX, stdin);
    cortar_nl(busca);
    limpar_tela();

    printf("----: pesquisar discente :----\n\n");
    for (int i = 0; i < qtd_cads; ++i) {
        strncpy(buff, discentes[i].nome, strlen(busca));
        if (strcmp(busca, buff) == 0) {
            imprimir_discente(discentes[i]);
        }
    }
}

void salvar_curso(char *onde, Curso curso) {
    FILE *f = fopen(onde, "a");
    fprintf(f, "%s,%d,%d,%d,%d\n", curso.nome, curso.codigo, curso.horas, curso.vagas, curso.numero_participantes);
    fclose(f);
}

void cadastrar_curso(char *onde) {
    Curso curso = {0};
    char buff[TAM_MAX];

    printf("Nome: ");
    fgets(curso.nome, TAM_MAX, stdin);
    cortar_nl(curso.nome);

    printf("Codigo: ");
    fgets(buff, TAM_MAX, stdin);
    cortar_nl(buff);
    curso.codigo = atoi(buff);

    printf("Horas: ");
    fgets(buff, TAM_MAX, stdin);
    cortar_nl(buff);
    curso.horas = atoi(buff);

    printf("Vagas: ");
    fgets(buff, TAM_MAX, stdin);
    cortar_nl(buff);
    curso.vagas = atoi(buff);

    printf("Numero de Participantes: ");
    fgets(buff, TAM_MAX, stdin);
    cortar_nl(buff);
    curso.numero_participantes = atoi(buff);

    salvar_curso(onde, curso);
}

Curso *popular_cursos(int qtd_cads, char *onde) {
    Curso *cursos = malloc(sizeof(Curso) * qtd_cads);
    char buff[TAM_MAX];

    FILE *f = fopen(onde, "r");
    if (f == NULL) {
        fprintf(stderr, "popular_cursos(): erro: nao foi possivel abrir arquivo: %s\n", onde);
        exit(1);
    };

    for (int i = 0; i < qtd_cads; ++i) {
        extrair_item(f, buff);
        strcpy(cursos[i].nome, buff);

        extrair_item(f, buff);
        cursos[i].codigo = atoi(buff);

        extrair_item(f, buff);
        cursos[i].horas = atoi(buff);

        extrair_item(f, buff);
        cursos[i].vagas = atoi(buff);

        extrair_item(f, buff);
        cursos[i].numero_participantes = atoi(buff);
    }

    return cursos;
}

void exibir_cursos(char *onde) {
    int qtd_cads = contar_cadastros(onde);
    Curso *cursos = popular_cursos(qtd_cads, onde);

    for (int i = 0; i < qtd_cads; ++i) {
        imprimir_curso(cursos[i]);
    }
}

void remover_curso(char *onde) {
    int qtd_cads = contar_cadastros(onde);
    char buff[TAM_MAX];

    Curso *cursos = popular_cursos(qtd_cads, onde);

    printf("+--------------------------------+\n");
    for (int i = 0; i < qtd_cads; ++i) {
        printf("│ %d │ %s\n", i, cursos[i].nome);
    }
    printf("+--------------------------------+\n");

    printf("Digite o numero identificador do curso:\n");
    printf("> ");

    fgets(buff, TAM_MAX, stdin);
    cortar_nl(buff);

    for (int k = 0; k < qtd_cads; ++k) {
        if (strcmp(cursos[k].nome, cursos[atoi(buff)].nome) == 0) continue;
        salvar_curso("tmp.csv", cursos[k]);
    }

    rename("tmp.csv", onde);
}

void salvar_turma(char *onde, Turma turma) {
    FILE *f = fopen(onde, "a");

    fprintf(f, "%d,%d,%d,%d,%d,", turma.numero, turma.codigo, turma.ano, turma.horas_participacao, turma.qtd_cpf);
    for (int i = 0; i < turma.qtd_cpf; i++) {
        fprintf(f, "%s,%f", turma.cpf[i], turma.notas[i]);
        if (i < (turma.qtd_cpf + 1)) fprintf(f, ",");
    }

    fprintf(f, "\n");

    fclose(f);
}

void cadastrar_turma(char *onde[]) {
    Turma turma = {0};
    char buff[TAM_MAX];
    
    printf("numero: ");
    fgets(buff, TAM_MAX, stdin);
    cortar_nl(buff);
    turma.numero = atoi(buff);

    printf("codigo: ");
    fgets(buff, TAM_MAX, stdin);
    cortar_nl(buff);
    turma.codigo = atoi(buff);

    printf("ano: ");
    fgets(buff, TAM_MAX, stdin);
    cortar_nl(buff);
    turma.ano = atoi(buff);

    printf("horas de participacao: ");
    fgets(buff, TAM_MAX, stdin);
    cortar_nl(buff);
    turma.horas_participacao = atoi(buff);
    
    int qtd_disc = 0;
    int qtd_cads = contar_cadastros(onde[ARQ_DISCENTES]);
    Discente *discentes = popular_discentes(qtd_cads, onde[ARQ_DISCENTES]);

    static char *cpfs_adicionados[TAM_MAX];

    for (;;) {
        limpar_tela();
        printf("----: adicionar discente :----\n\n");
        for (int i = 0; i < qtd_cads; ++i) {
            int adicionado = 0;
            for (int j = 0; j < qtd_disc; ++j) {
                if (strcmp(discentes[i].cpf, cpfs_adicionados[j]) == 0) {
                    adicionado = 1;
                }
            }

            if (!adicionado) {
                printf("+ %s (%s)\n", discentes[i].cpf, discentes[i].nome);
            }
        }

        printf("+---------: adicionados :---------+\n\n");

        if (qtd_disc == 0) printf("nenhum discente adicionado!!\n\n");

        for (int k = 0; k < qtd_disc; ++k) {
            printf("+ %s\n", cpfs_adicionados[k]);
        }

        printf("\n");
        printf("digite cpf do discente para adicionar a turma (q para finalizar):\n");
        printf("> ");
        
        fgets(buff, TAM_MAX, stdin);
        cortar_nl(buff);

        if (*buff == 'q') break;
        
        for (int i = 0; i < qtd_cads; ++i) {
            if (strcmp(discentes[i].cpf, buff) == 0) {
                cpfs_adicionados[qtd_disc] = malloc(strlen(buff));
                strcpy(cpfs_adicionados[qtd_disc], buff);
                qtd_disc++;
            }
        }
    }
    turma.cpf = cpfs_adicionados;
    turma.qtd_cpf = qtd_disc;
    turma.notas = malloc(sizeof(float) * qtd_disc);

    for (int k = 0; k < qtd_disc; ++k) {
        limpar_tela();
        printf("----: registrar notas :----\n\n");

        printf("+ %s\n\n", cpfs_adicionados[k]);
        printf("digite nota do discente:\n");
        printf("> ");

        fgets(buff, TAM_MAX, stdin);
        cortar_nl(buff);
        turma.notas[k] = atof(buff);

    }

    salvar_turma(onde[ARQ_TURMAS], turma);
}

Turma *popular_turmas(int qtd_cads, char *onde) {
    Turma *turmas = malloc(sizeof(Turma) * qtd_cads);

    char buff[TAM_MAX];

    FILE *f = fopen(onde, "r");
    if (f == NULL) {
        fprintf(stderr, "popular_turmas(): erro: nao foi possivel abrir arquivo: %s\n", onde);
        exit(1);
    };

    for (int i = 0; i < qtd_cads; ++i) {
        extrair_item(f, buff);
        turmas[i].numero = atoi(buff);

        extrair_item(f, buff);
        turmas[i].codigo = atoi(buff);

        extrair_item(f, buff);
        turmas[i].ano = atoi(buff);

        extrair_item(f, buff);
        turmas[i].horas_participacao = atoi(buff);

        extrair_item(f, buff);
        int qtd_disc = atoi(buff);

        turmas[i].cpf = malloc(qtd_disc);
        turmas[i].notas = malloc(sizeof(float) * qtd_disc);

        for (int j = 0; j < qtd_disc; ++j) {
            extrair_item(f, buff);

            turmas[i].cpf[j] = malloc(sizeof(char) * strlen(buff));

            strcpy(turmas[i].cpf[j], buff);

            extrair_item(f, buff);
            turmas[i].notas[j] = atof(buff);
        }

        turmas[i].qtd_cpf = qtd_disc;
    }


    return turmas;
}

void exibir_turmas(char *onde[]) {
    int qtd_cads = contar_cadastros(onde[ARQ_TURMAS]);
    Turma *turmas = popular_turmas(qtd_cads, onde[ARQ_TURMAS]);

    for (int i = 0; i < qtd_cads; ++i) {
        imprimir_turma(turmas[i], onde);
    }
}

void remover_turma(char *onde[]) {
    int qtd_cads = contar_cadastros(onde[ARQ_TURMAS]);
    char buff[TAM_MAX];

    Turma *turmas = popular_turmas(qtd_cads, onde[ARQ_TURMAS]);

    printf("+--------------------------------+\n");
    for (int i = 0; i < qtd_cads; ++i) {
        printf("│ %d │ %d\n", i, turmas[i].codigo);
    }
    printf("+--------------------------------+\n");

    printf("Digite o numero identificador da turma:\n");
    printf("> ");

    fgets(buff, TAM_MAX, stdin);
    cortar_nl(buff);

    for (int k = 0; k < qtd_cads; ++k) {
        if (turmas[k].codigo == turmas[atoi(buff)].codigo) continue;
        salvar_turma("tmp.csv", turmas[k]);
    }
    rename("tmp.csv", onde[ARQ_TURMAS]);
}

int main(void) {
    Menus menu = PRINCIPAL;

    char *arquivos[] = {
        "discentes.csv", // ARQ_DISCENTES
        "cursos.csv",    // ARQ_CURSOS
        "turmas.csv"     // ARQ_TURMAS
    };
    char op[8];
    
    for (;;) {
        limpar_tela();
        switch (menu) {
            case PRINCIPAL:
                printf("----: menu principal :----\n\n");
                printf("1. discentes\n");
                printf("2. cursos\n");
                printf("3. turmas\n");
                printf("4. relatorios\n");
                printf("5. sair\n");
                printf("\n> ");
                fgets(op, 8, stdin);
                
                switch (*op) {
                    case '1': menu = DISCENTES; break;
                    case '2': menu = CURSOS; break;
                    case '3': menu = TURMAS; break;
                    case '4': menu = RELATORIOS; break;
                    case '5': menu = SAIR; break;
                    default: menu = INVALIDO;
                }

                break;
            case DISCENTES:
                printf("----: discentes :----\n\n");
                printf("1. cadastrar discente\n");
                printf("2. remover discente\n");

                printf("\n> ");

                fgets(op, 8, stdin);
                if (*op == '1') {
                    limpar_tela();
                    printf("----: cadastrar discente :----\n\n");
                    cadastrar_discente(arquivos[ARQ_DISCENTES]);
                } else if (*op == '2') {
                    limpar_tela();
                    printf("----: remover discente :----\n\n");
                    remover_discente(arquivos[ARQ_DISCENTES]);
                }
                
                menu = PRINCIPAL;
                break;
            case CURSOS:
                printf("----: cursos :----\n\n");
                printf("1. cadastrar curso\n");
                printf("2. remover curso\n");

                printf("\n> ");

                fgets(op, 8, stdin);
                
                if (*op == '1') {
                    limpar_tela();
                    printf("----: cadastrar curso :----\n\n");
                    cadastrar_curso(arquivos[ARQ_CURSOS]);
                } else if (*op == '2') {
                    limpar_tela();
                    printf("----: remover curso :----\n\n");
                    remover_curso(arquivos[ARQ_CURSOS]);
                }

                menu = PRINCIPAL;
                break;
            case TURMAS:
                limpar_tela();
                printf("----: turmas :----\n\n");
                printf("1. cadastrar turma\n");
                printf("2. remover turma\n");
                printf("\n> ");

                fgets(op, 8, stdin);

                if (*op == '1') {
                    limpar_tela();
                    printf("----: cadastrar turma :----\n\n");
                    cadastrar_turma(arquivos);
                } else if (*op == '2') {
                    limpar_tela();
                    printf("----: remover turma :----\n\n");
                    remover_turma(arquivos);
                }

                menu = PRINCIPAL;
                break;
            case RELATORIOS:
                limpar_tela();
                printf("----: relatorios :----\n\n");
                printf("1. discentes\n");
                printf("2. cursos\n");
                printf("3. turmas\n");
                printf("\n> ");

                fgets(op, 8, stdin);
                switch (*op) {
                    case '1':
                        limpar_tela();
                        printf("----: listagem discentes :----\n\n");
                        printf("1. lista completa\n");
                        printf("2. pesquisar\n");
                        printf("\n> ");

                        fgets(op, 8, stdin);

                        if (*op == '1') {
                            limpar_tela();
                            printf("----: listagem discentes :----\n");
                            exibir_discentes(arquivos[ARQ_DISCENTES]);
                            getchar();
                        } else if (*op == '2') {
                            limpar_tela();
                            printf("----: pesquisar discente :----\n");
                            pesquisar_discente(arquivos[ARQ_DISCENTES]);
                            getchar();
                        }
                        break;
                    case '2':
                        limpar_tela();
                        printf("----: listagem cursos :----\n\n");
                        exibir_cursos(arquivos[ARQ_CURSOS]);
                        getchar();
                        break;
                    case '3':
                        limpar_tela();
                        printf("----: listagem turmas :----\n\n");
                        printf("1. todas as turmas\n");
                        //printf("2. por desempenho\n");
                        printf("x. relatorio detalhado\n");

                        fgets(op, 8, stdin);

                        switch (*op) {
                            case '1':
                                limpar_tela();
                                printf("----: todas as turmas :----\n\n");
                                exibir_turmas(arquivos);
                                break;
                            case '2':
                                limpar_tela();
                                printf("----: turmas por desempenho :----\n\n");

                                getchar();
                                break;
                        }

                        getchar();
                        break;
                    default:
                        menu = INVALIDO;
                }

                menu = PRINCIPAL;
                break;
            case SAIR: return 0;
            default:
                printf("----: erro :----\n");
                printf("opção inválida!!\n");
                getchar();

                menu = PRINCIPAL;
        }
    }
    
    return 0;
}
