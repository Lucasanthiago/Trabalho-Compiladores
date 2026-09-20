/* scanner_manual.c -- analisador lexico implementado a partir do DFA minimo
 *
 * A tabela delta do DFA minimo vive em tabela.h. Este arquivo e o motor:
 * percorre a entrada aplicando a regra do maior casamento possivel,
 * conta linha e coluna, descarta espacos e comentarios e reporta erro lexico.
 *
 * Compilar:  gcc -o scanner_manual scanner_manual.c
 * Usar:      ./scanner_manual arquivo.cm
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabela.h"

static char *buf;
static long  tam;
static long  pos = 0;
static int   linha = 1, coluna = 1;
static int   erros = 0;

/* avanca uma posicao mantendo linha e coluna coerentes */
static void avanca(void) {
    if (buf[pos] == '\n') { linha++; coluna = 1; }
    else                  { coluna++; }
    pos++;
}

/* espacos em branco e comentarios nao geram token */
static int descarta_ignoraveis(void) {
    int mudou = 1;
    while (mudou) {
        mudou = 0;
        while (pos < tam && (buf[pos] == ' ' || buf[pos] == '\t' ||
                             buf[pos] == '\n' || buf[pos] == '\r')) {
            avanca(); mudou = 1;
        }
        if (pos + 1 < tam && buf[pos] == '/' && buf[pos + 1] == '*') {
            int l0 = linha, c0 = coluna;
            avanca(); avanca();
            while (pos + 1 < tam && !(buf[pos] == '*' && buf[pos + 1] == '/'))
                avanca();
            if (pos + 1 >= tam) {
                fprintf(stderr, "ERRO LEXICO linha %d coluna %d: "
                        "comentario aberto e nao fechado\n", l0, c0);
                erros++;
                pos = tam;
                return 0;
            }
            avanca(); avanca();
            mudou = 1;
        }
    }
    return pos < tam;
}

int main(int argc, char **argv) {
    FILE *f = stdin;

    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    if (argc > 1) {
        f = fopen(argv[1], "rb");
        if (!f) { perror(argv[1]); return 1; }
    }
    /* le a entrada inteira: o recuo do maior casamento precisa voltar atras */
    tam = 0;
    long cap = 4096;
    buf = malloc(cap);
    int ch;
    while ((ch = fgetc(f)) != EOF) {
        if (tam + 1 >= cap) { cap *= 2; buf = realloc(buf, cap); }
        buf[tam++] = (char)ch;
    }
    buf[tam] = '\0';
    if (f != stdin) fclose(f);

    printf("== scanner implementado a partir do DFA minimo -- %s ==\n",
           argc > 1 ? argv[1] : "entrada padrao");
    printf("%-5s %-5s %-10s %s\n", "LIN", "COL", "TOKEN", "LEXEMA");
    printf("--------------------------------------------\n");

    while (descarta_ignoraveis()) {
        int  estado = INICIAL;
        long ini = pos;
        int  lin_ini = linha, col_ini = coluna;

        /* maior casamento: guarda o ultimo estado final visto e onde ele foi */
        const char *ult_token = NULL;
        long ult_pos = -1;
        int  ult_linha = linha, ult_coluna = coluna;

        long p = pos;
        int  l = linha, c = coluna;
        while (p < tam) {
            int k = classe_de[(unsigned char)buf[p]];
            if (k < 0) break;
            int prox = delta[estado][k];
            if (prox < 0) break;
            estado = prox;
            if (buf[p] == '\n') { l++; c = 1; } else { c++; }
            p++;
            if (token_de[estado]) {
                ult_token  = token_de[estado];
                ult_pos    = p;
                ult_linha  = l;
                ult_coluna = c;
            }
        }

        if (ult_token) {                      /* recua ate o ultimo final */
            long n = ult_pos - ini;
            printf("%-5d %-5d %-10s %.*s\n", lin_ini, col_ini, ult_token,
                   (int)n, buf + ini);
            pos = ult_pos; linha = ult_linha; coluna = ult_coluna;
        } else {                              /* travou sem nenhum final */
            fprintf(stderr, "ERRO LEXICO linha %d coluna %d: "
                    "caractere invalido \"%c\"\n", linha, coluna, buf[pos]);
            erros++;
            avanca();
        }
    }

    printf("--------------------------------------------\n");
    printf("erros lexicos: %d\n", erros);
    free(buf);
    return erros ? 1 : 0;
}
