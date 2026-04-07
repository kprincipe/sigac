#ifndef TURMA_H
#define TURMA_H

#include "common.h"

void cadastrar_turma(char **arquivos);
int popular_turmas(Turma *turmas, char *arquivo);
void mostrar_turmas(Turma *turmas, int qtd_turmas);
void remover_uma_turma(int indice, char *arquivo);
void remover_turma(char *arquivo);
void editar_turma(char *arquivo[]);
void salvar_turma(Turma turma, char *arquivo); // salva dados de turma em um arquivo

#endif // TURMA_H
