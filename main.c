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
    int nota;
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
                printf("----: menu principal :----\n");
                printf("1. Discentes\n");
                printf("2. Cursos\n");
                printf("x. Turmas\n");
                printf("4. Relatorios\n");
                printf("5. Sair\n");
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
                printf("----: discentes :----\n");
                printf("1. Cadastrar discente\n");
                printf("2. Remover discente\n");
                printf("\n> ");

                fgets(op, 8, stdin);
                if (*op == '1') {
                    limpar_tela();
                    printf("----: cadastrar discente :----\n");
                    cadastrar_discente(arquivos[ARQ_DISCENTES]);
                } else if (*op == '2') {
                    limpar_tela();
                    printf("----: remover discente :----\n");
                    remover_discente(arquivos[ARQ_DISCENTES]);
                }
                
                menu = PRINCIPAL;
                break;
            case CURSOS:
                printf("----: cursos :----\n");
                printf("1. Cadastrar curso\n");
                printf("x. Remover curso\n");
                printf("\n> ");

                fgets(op, 8, stdin);
                
                if (*op == '1') {
                    limpar_tela();
                    printf("----: cadastrar curso :----\n");
                    cadastrar_curso(arquivos[ARQ_CURSOS]);
                }

                menu = PRINCIPAL;
                break;
            case RELATORIOS:
                limpar_tela();
                printf("----: relatorios :----\n\n");
                printf("1. Discentes\n");
                printf("2. Cursos\n");
                printf("x. Turmas\n");
                printf("\n> ");

                fgets(op, 8, stdin);

                if (*op == '1') {
                    limpar_tela();
                    printf("----: relatorios :----\n\n");
                    exibir_discentes(arquivos[ARQ_DISCENTES]);
                    getchar();
                } else if (*op == '2') {
                    limpar_tela();
                    printf("----: relatorios :----\n\n");
                    exibir_cursos(arquivos[ARQ_CURSOS]);
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
