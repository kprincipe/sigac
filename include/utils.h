#ifndef UTILS_H
#define UTILS_H

void cortar_nl(char *s);                    // funcao que elimina caractere '\n' do final de uma string
int contar_cadastros(char *onde);           // contar quantidade de linhas (cadastros) existentes em arquivo
void extrair_item(FILE *f, char *dst);      // extrair string com dado individual entre virgulas em um arquivo csv

#ifdef UTILS_IMPLEMENTATION

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

#endif // UTILS_IMPLEMENTATION
#endif // UTILS_H
