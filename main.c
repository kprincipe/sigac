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

typedef struct {
    char nome[TAM_MAX];
    char cpf[TAM_MAX];
    char idade[TAM_MAX];
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

void imprimir_discente(Discente discente) {
    printf("----------------\n");
    printf("| Nome  | %s\n", discente.nome);
    printf("| CPF   | %s\n", discente.cpf);
    printf("| Idade | %s\n", discente.idade);
    printf("----------------\n");
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
        strcpy(discentes[i].idade, buff);
    }

    return discentes;
}

void exibir_discentes(char *onde) {
    int qtd_cads = contar_cadastros(onde);
    Discente *discentes = popular_discentes(qtd_cads, onde);;

    if (discentes )
    
    for (int i = 0; i < qtd_cads; ++i) {
        printf("+ %s\n+ %s\n+ %s\n------------------------\n", discentes[i].nome, discentes[i].cpf, discentes[i].idade);
    }
}

void salvar_discente(char *onde, Discente discente) {
    FILE *f = fopen(onde, "a");
    fprintf(f, "%s,%s,%s\n", discente.nome, discente.cpf, discente.idade);
    fclose(f);
}

void cortar_nl(char *s) {
    int i = 0;
    while (s[i] != '\0') i++;
    s[i - 1] = '\0';
}

void cadastrar_discente(char *onde) {
    Discente discente = {0};

    printf("Nome: ");
    fgets(discente.nome, TAM_MAX, stdin);
    cortar_nl(discente.nome);

    printf("CPF: ");
    fgets(discente.cpf, TAM_MAX, stdin);
    cortar_nl(discente.cpf);

    printf("Idade: ");
    fgets(discente.idade, TAM_MAX, stdin);
    cortar_nl(discente.idade);

    salvar_discente(onde, discente);
}

int main(void) {
    Menus menu = PRINCIPAL;

    char *arquivo = "discentes.csv";
    char op[8];
    
    for (;;) {
        limpar_tela();
        switch (menu) {
            case PRINCIPAL:
                printf("----: menu principal :----\n");
                printf("1. Discentes\n");
                printf("2. Cursos\n");
                printf("x. Turmas\n");
                printf("x. Relatorios\n");
                printf("5. Sair\n");
                printf("\n> ");
                fgets(op, 8, stdin);

                if (*op == '1') menu = DISCENTES;
                else if (*op == '2') menu = CURSOS;
                else if (*op == '3') menu = TURMAS;
                else if (*op == '4') menu = RELATORIOS;
                else if (*op == '5') menu = SAIR;
                else menu = INVALIDO;

                break;
            case DISCENTES:
                printf("----: discentes :----\n");
                printf("1. Cadastrar discente\n");
                printf("2. Remover discente\n");
                printf("3. Listar discentes\n");
                printf("\n> ");

                fgets(op, 8, stdin);
                if (*op == '1') {
                    limpar_tela();
                    printf("----: cadastrar discente :----\n");
                    cadastrar_discente(arquivo);
                }
                if (*op == '3') {
                    limpar_tela();
                    printf("----: discentes :----\n\n");
                    exibir_discentes(arquivo);
                    getchar();
                }
                
                menu = PRINCIPAL;
                break;
            case CURSOS:
                printf("----: cursos :----\n");
                printf("1. Listar cursos\n");
                printf("\n> ");

                fgets(op, 8, stdin);
                
                if (*op == '1') {
                    limpar_tela();
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
