#ifndef DISCENTE_H
#define DISCENTE_H

#include "common.h"

void cadastrar_discente(char *arquivo);
int popular_discentes(Discente *discentes, char *arquivo);
void mostrar_discentes(Discente *discentes, int qtd_discentes);
void mostrar_discente(Discente discente);
void remover_um_discente(char *arquivo, int i);
void remover_discente(char *arquivo);
void editar_discente(char *arquivo);
void salvar_discente(Discente discente, char *arquivo); // salva dados de discente em um arquivo
void pesquisar_discente(char *arquivo);

#endif // DISCENTE_H
