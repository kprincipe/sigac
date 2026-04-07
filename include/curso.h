#ifndef CURSO_H
#define CURSO_H

void cadastrar_curso(char *arquivo);
int popular_cursos(Curso *cursos, char *arquivo);
void mostrar_cursos(Curso *cursos, int qtd_cursos);
void remover_um_curso(char *arquivo, int i);
void remover_curso(char *arquivo);
void editar_curso(char *arquivo);
void salvar_curso(Curso curso, char *arquivo); // salva dados de curso em um arquivo

#endif // CURSO_H
