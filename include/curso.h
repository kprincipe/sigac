#ifndef CURSO_H
#define CURSO_H

void cadastrar_curso(char *arquivo);                // exibe interface de coleta dados de curso do usuario
int popular_cursos(Curso *cursos, char *arquivo);   // le arquivo de cursos e popula vetor com as informacoes
void mostrar_cursos(Curso *cursos, int qtd_cursos); // exibe todos os cursos
void remover_um_curso(char *arquivo, int i);        // remove totalmente dados de curso no indice i do arquivo sem mensagens (printf)
void remover_curso(char *arquivo);                  // exibe interface de remocao de curso
void editar_curso(char *arquivo);                   // exibe interface que altera campos de curso e reescreve no arquivo com as novas informacoes
void salvar_curso(Curso curso, char *arquivo);      // salva dados de curso em um arquivo csv (comma separated values)

#endif // CURSO_H
