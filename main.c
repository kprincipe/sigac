#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#define LIMPAR_TELA() system("cls")
#else
#define LIMPAR_TELA() system("clear")
#endif

#define CABECALHO() printf("-- sistema TopArtBare --\n\n")

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

// funcao que elimina caractere '\n' do final de uma string
void cortar_nl(char *s);

// contar quantidade de linhas (cadastros) existentes em arquivo
int contar_cadastros(char *onde);
int contar_cadastros(char *onde);
void extrair_item(FILE *f, char *dst);

void cadastrar_curso(char *arquivo);
int popular_cursos(Curso *cursos, char *arquivo);
void mostrar_cursos(Curso *cursos, int qtd_cursos);
void remover_curso(char *arquivo);
void editar_curso(char *arquivo);
void salvar_curso(Curso curso, char *arquivo); // salva dados de curso em um arquivo

void cadastrar_discente(char *arquivo);
int popular_discentes(Discente *discentes, char *arquivo);
void mostrar_discentes(Discente *discentes, int qtd_discentes);
void mostrar_discente(Discente discente);
void remover_um_discente(char *arquivo, int i);
void remover_discente(char *arquivo);
void editar_discente(char *arquivo);
void salvar_discente(Discente discente, char *arquivo); // salva dados de discente em um arquivo
void pesquisar_discente(char *arquivo);

void cadastrar_turma(char **arquivos);
int popular_turmas(Turma *turmas, char *arquivo);
void mostrar_turmas(Turma *turmas, int qtd_turmas);
void remover_turma(char *arquivo);
void editar_turma(char *arquivo);
void salvar_turma(Turma turma, char *arquivo); // salva dados de turma em um arquivo

void cortar_nl(char *s) {
    int i = 0;
    while (s[i] != '\0') i++;
    s[i - 1] = '\0';
}

int contar_cadastros(char *arquivo) {
    int c = 0;

    FILE *f = fopen(arquivo, "r");
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

// le arquivo de cursos e popula vetor com as informacoes
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

// coleta dados de curso do usuario
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

void remover_uma_turma(char *arquivo, int i) {
    Turma turmas[TAM_MAX];
    int qtd_turmas = popular_turmas(turmas, arquivo);
    int qtd_nums = 0, nums[TAM_MAX];

    for (int k = 0; k < qtd_turmas; ++k) {
        if (turmas[k].numero == nums[i]) continue;
        salvar_turma(turmas[k], "tmp.csv");
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

void editar_turma(char *arquivo) {
    printf("erro: nao implementado\n");
    exit(1);
}

void salvar_turma(Turma turma, char *arquivo) {
    FILE *f = fopen(arquivo, "a");

    fprintf(f, "%s,%d,%d,%d,%f,%f\n", turma.cpf, turma.numero, turma.codigo_curso, turma.ano, turma.nota, turma.horas_participacao);

    fclose(f);
}

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
                    printf("----: turmas :----\n\n");
                    mostrar_turmas(turmas, qtd_turmas);
                    editar_turma(arquivos[ARQ_TURMAS]);
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
