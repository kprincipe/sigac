#ifndef DISCENTE_H
#define DISCENTE_H

#include "common.h"

void cadastrar_discente(char *arquivo);                             // exibe interface de cadastro de discentes
int popular_discentes(Discente *discentes, char *arquivo);          // le arquivo de discentes e popula vetor com as informacoes
void mostrar_discentes(Discente *discentes, int qtd_discentes);     // exibe o nome de todos os discentes
void mostrar_discente(Discente discente);                           // exibe o nome de um discente determinado
void remover_um_discente(char *arquivo, int i);                     // remove dados realacionados ao discente no indice i de um vetor de `struct Discente` sem imprimir mensagens
void remover_discente(char *arquivo);                               // exibe interface de remocao de discentes
void editar_discente(char *arquivo);                                // exibe interface de edicao de discentes
void salvar_discente(Discente discente, char *arquivo);             // salva dados de discente em um arquivo
void pesquisar_discente(char *arquivo);

#endif // DISCENTE_H
