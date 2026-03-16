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
    float nota;
    int horas_participacao;
    char cpf[TAM_MAX];
} Turma;

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

void imprimir_turma(Turma turma) {
    printf("+--------------------------------+\n");
    printf("│ Numero     │ %d\n", turma.numero);
    printf("│ Codigo     │ %d\n", turma.codigo);
    printf("│ ano        │ %d h\n", turma.ano);
    printf("│ nota       │ %.2f\n", turma.nota);
    printf("│ hrs. part. │ %d\n", turma.horas_participacao);
    printf("│ cpf        │ %s\n", turma.cpf);
    printf("+--------------------------------+\n\n");
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

    getchar();
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
    fprintf(f, "%d,%d,%d,%f,%d,%s\n", turma.numero, turma.codigo, turma.ano, turma.nota, turma.horas_participacao, turma.cpf);
    fclose(f);
}

void cadastrar_turma(char *onde) {
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

    printf("nota: ");
    fgets(buff, TAM_MAX, stdin);
    cortar_nl(buff);
    turma.nota = atof(buff);

    printf("horas de participacao: ");
    fgets(buff, TAM_MAX, stdin);
    cortar_nl(buff);
    turma.horas_participacao = atoi(buff);

    printf("cpf: ");
    fgets(turma.cpf, TAM_MAX, stdin);
    cortar_nl(turma.cpf);

    salvar_turma(onde, turma);
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
        turmas[i].nota = atof(buff);

        extrair_item(f, buff);
        turmas[i].horas_participacao = atoi(buff);

        extrair_item(f, buff);
        strcpy(turmas[i].cpf, buff);
    }

    return turmas;
}

void exibir_turmas(char *onde) {
    int qtd_cads = contar_cadastros(onde);
    Turma *turmas = popular_turmas(qtd_cads, onde);

    for (int i = 0; i < qtd_cads; ++i) {
        imprimir_turma(turmas[i]);
    }
}

void remover_turma(char *onde) {
    int qtd_cads = contar_cadastros(onde);
    char buff[TAM_MAX];

    Turma *turmas = popular_turmas(qtd_cads, onde);

    printf("+--------------------------------+\n");
    for (int i = 0; i < qtd_cads; ++i) {
        printf("│ %d │ %d\n", i, turmas[i].numero);
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

    rename("tmp.csv", onde);
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
                    cadastrar_turma(arquivos[ARQ_TURMAS]);
                } else if (*op == '2') {
                    limpar_tela();
                    printf("----: remover turma :----\n\n");
                    remover_turma(arquivos[ARQ_TURMAS]);
                }

                menu = PRINCIPAL;
                break;
            case RELATORIOS:
                limpar_tela();
                printf("----: relatorios :----\n\n");
                printf("1. discentes\n");
                printf("2. cursos\n");
                printf("3. pesquisar discente\n");
                printf("4. turmas\n");
                printf("\n> ");

                fgets(op, 8, stdin);

                if (*op == '1') {
                    limpar_tela();
                    printf("----: listagem discentes :----\n\n");
                    exibir_discentes(arquivos[ARQ_DISCENTES]);
                    getchar();
                } else if (*op == '2') {
                    limpar_tela();
                    printf("----: listagem cursos :----\n\n");
                    exibir_cursos(arquivos[ARQ_CURSOS]);
                    getchar();
                } else if (*op == '3') {
                    limpar_tela();
                    printf("----: pesquisar discente :----\n");
                    pesquisar_discente(arquivos[ARQ_DISCENTES]);
                } else if (*op == '4') {
                    limpar_tela();
                    printf("----: listagem turmas :----\n");
                    exibir_turmas(arquivos[ARQ_TURMAS]);
                    getchar();
                }

                menu = PRINCIPAL;
                break;
            case SAIR:
                    printf("tchauu!\n");
                    getchar();
                    return 0;
                break;
            default:
                printf("----: erro :----\n");
                printf("opção inválida!!\n");
                getchar();

                menu = PRINCIPAL;
        }
    }
    
    return 0;
}
