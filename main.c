#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#define QTD_OP 5

#define TAM_MAX 256

typedef enum {
    PRINCIPAL = 0,
    DISCENTES,
    CURSOS,
    TURMAS,
    RELATÓRIOS,
    INVALIDO,
    SAIR
} Menus;

typedef struct {
    char cpf[TAM_MAX];
    char nome[TAM_MAX];
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

void imprimir_discente(Discente discente) {
    printf("----------------\n");
    printf("| Nome  | %s\n", discente.nome);
    printf("| CPF   | %s\n", discente.cpf);
    printf("| Idade | %d\n", discente.idade);
    printf("----------------\n");
}

void salvar_discente(char *onde, Discente discente) {
    FILE *f = fopen(onde, "ab");

    fseek(f, 0, sizeof(Discente));
    fwrite(&discente, sizeof(Discente), 1, f);
    
    fclose(f);
}

Discente abrir_discente(int qual, char *onde) {
    FILE *f = fopen(onde, "rb");

    Discente discente = {0};

    fseek(f, sizeof(Discente) * qual, SEEK_SET);
    fread(&discente, sizeof(Discente), 1, f);
    
    fclose(f);

    return discente;
}

void cortar_nl(char *s) {
    int i = 0;
    while (s[i] != '\0') i++;
    s[i - 1] = '\0';
}

Discente cadastrar_discente() {
    Discente discente = { 0 };

    printf("Nome: ");
    fgets(discente.nome, TAM_MAX, stdin);
    cortar_nl(discente.nome);

    printf("CPF: ");
    fgets(discente.cpf, TAM_MAX, stdin);
    cortar_nl(discente.cpf);

    printf("Idade: ");
    scanf("%d", &discente.idade);

    salvar_discente("discentes.db", discente);

    return discente;
}

int main(void) {
    Menus menu = PRINCIPAL;

    char op[8];
    
    for (;;) {
        system("clear");
        switch (menu) {
            case PRINCIPAL:
                printf("----: menu principal :----\n");
                printf("1. Discentes\n");
                printf("2. Sair\n");
                printf("\n> ");
                fgets(op, 8, stdin);

                if (*op == '1') menu = DISCENTES;
                if (*op == '2') menu = SAIR;
                break;
            case DISCENTES:
                printf("----: discentes :----\n");
                printf("1. Cadastrar discente\n");
                printf("2. Remover discente\n");
                printf("3. Listar discentes\n");
                printf("\n> ");
                fgets(op, 8, stdin);
                
                menu = PRINCIPAL;
                break;
            case SAIR:
                    printf("tchauu!\n");
                    exit(1);
                break;
            default:
                printf("opção inválida: %s\n", op);
                getchar();
                menu = PRINCIPAL;
        }
    }

    // Discente discente = cadastrar_discente();

    // Discente discente = abrir_discente(2, "discentes.db");
    // imprimir_discente(discente);
    
    return 0;
}
