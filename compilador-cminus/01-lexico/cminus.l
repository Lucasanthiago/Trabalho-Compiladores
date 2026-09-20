/* cminus.l -- analisador lexico do C-Minus (BNF completa, 29 regras)
 *
 * Compilar:  flex cminus.l && gcc lex.yy.c -o scanner_flex
 * Usar:      ./scanner_flex arquivo.cm
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int  linha = 1;
int  coluna = 1;
int  erros = 0;
int  com_linha = 0, com_coluna = 0;   /* onde o comentario aberto comecou */

/* imprime um par (lexema, token) e avanca a coluna */
static void emite(const char *token) {
    printf("%-5d %-5d %-10s %s\n", linha, coluna, token, yytext);
    coluna += yyleng;
}

static void erro(const char *msg) {
    fprintf(stderr, "ERRO LEXICO linha %d coluna %d: %s \"%s\"\n",
            linha, coluna, msg, yytext);
    erros++;
    coluna += yyleng;
}
%}

%option noyywrap
%option nounput
%option noinput

letra    [a-zA-Z]
digito   [0-9]
ID       {letra}+
NUM      {digito}+
espaco   [ \t]

%x COMENTARIO

%%

    /* --- comentarios: /* ... *\/ nao geram token --- */
"/*"                { com_linha = linha; com_coluna = coluna;
                      coluna += yyleng; BEGIN(COMENTARIO); }
<COMENTARIO>"*/"    { coluna += yyleng; BEGIN(INITIAL); }
<COMENTARIO>\n      { linha++; coluna = 1; }
<COMENTARIO>.       { coluna++; }
<COMENTARIO><<EOF>> { fprintf(stderr,
                        "ERRO LEXICO linha %d coluna %d: comentario aberto e nao fechado\n",
                        com_linha, com_coluna);
                      erros++; BEGIN(INITIAL); yyterminate(); }

    /* --- palavras reservadas: antes de ID, para vencerem o empate --- */
"else"      { emite("ELSE"); }
"if"        { emite("IF"); }
"int"       { emite("INT"); }
"return"    { emite("RETURN"); }
"void"      { emite("VOID"); }
"while"     { emite("WHILE"); }

    /* --- operadores de dois caracteres antes dos de um --- */
"<="        { emite("LE"); }
">="        { emite("GE"); }
"=="        { emite("EQ"); }
"!="        { emite("NE"); }

"+"         { emite("PLUS"); }
"-"         { emite("MINUS"); }
"*"         { emite("TIMES"); }
"/"         { emite("OVER"); }
"<"         { emite("LT"); }
">"         { emite("GT"); }
"="         { emite("ASSIGN"); }
";"         { emite("SEMI"); }
","         { emite("COMMA"); }
"("         { emite("LPAREN"); }
")"         { emite("RPAREN"); }
"["         { emite("LBRACKET"); }
"]"         { emite("RBRACKET"); }
"{"         { emite("LBRACE"); }
"}"         { emite("RBRACE"); }

    /* --- numero colado em letra: erro lexico explicito --- */
{NUM}{letra}+   { erro("numero mal formado"); }

{ID}        { emite("ID"); }
{NUM}       { emite("NUM"); }

\n          { linha++; coluna = 1; }
{espaco}+   { coluna += yyleng; }

.           { erro("caractere invalido"); }

%%

int main(int argc, char **argv) {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) { perror(argv[1]); return 1; }
        printf("== scanner gerado com Flex -- %s ==\n", argv[1]);
    } else {
        printf("== scanner gerado com Flex -- entrada padrao ==\n");
    }
    printf("%-5s %-5s %-10s %s\n", "LIN", "COL", "TOKEN", "LEXEMA");
    printf("--------------------------------------------\n");

    yylex();

    printf("--------------------------------------------\n");
    printf("erros lexicos: %d\n", erros);
    return erros ? 1 : 0;
}
