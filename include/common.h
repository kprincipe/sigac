#ifndef COMMON_H
#define COMMON_H

#define TAM_MAX 256

#ifdef _WIN32
#define LIMPAR_TELA() system("cls")
#else
#define LIMPAR_TELA() system("clear")
#endif

#define CABECALHO() printf("-- sistema TopArtBare --\n\n")

typedef enum {
    PRINCIPAL = 0,
    DISCENTES,
    CURSOS,
    TURMAS,
    RELATORIOS,
    INVALIDO,
    SAIR
} Menus;

enum {
    ARQ_CURSOS,
    ARQ_DISCENTES,
    ARQ_TURMAS
};

// discentes (cpf, nome, idade)
typedef struct {
    char cpf[TAM_MAX];
    char nome[TAM_MAX];
    int idade;
} Discente;

// cursos (código, nome, horas, número de vagas, número de participantes)
typedef struct {
    char nome[TAM_MAX];
    int codigo;
    int horas;
    int numero_vagas;
    int numero_participantes;
} Curso;

// turmas (número, cpf, código, ano, nota, horas de participação)
typedef struct {
    char cpf[TAM_MAX];
    int numero;
    int codigo_curso;
    int ano;
    float nota;
    float horas_participacao;
} Turma;

#endif // COMMON_H
