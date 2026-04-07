#ifndef TURMA_H
#define TURMA_H

#include "common.h"

void cadastrar_turma(char **arquivos);                  // exibe interface de cadastro de turmas
int popular_turmas(Turma *turmas, char *arquivo);       // le arquivo de turmas e popula vetor com as informacoes
void mostrar_turmas(Turma *turmas, int qtd_turmas);     // exibe turmas em um vetor
void remover_uma_turma(int indice, char *arquivo);      // remove informacoes de turma de arquivo sem imprimir mensagens
void remover_turma(char *arquivo);                      // exibe interface de remocao de turmas
void editar_turma(char *arquivo[]);                     // exibe interface de edicao de turmas
void salvar_turma(Turma turma, char *arquivo);          // salva dados de turma em um arquivo

#endif // TURMA_H
