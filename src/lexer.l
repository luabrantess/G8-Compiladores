%{
#include "parser.tab.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int lexical_errors = 0;

static char* copiar_texto(const char* texto) {
    char* copia = malloc(strlen(texto) + 1);
    if (copia == NULL) {
        fprintf(stderr, "Erro interno: falha ao copiar identificador\n");
        exit(1);
    }

    strcpy(copia, texto);
    return copia;
}
%}

%option yylineno

%x COMMENT

%%

"//".* { /* Ignora comentarios de uma linha */ }

"/*"                 { BEGIN(COMMENT); }
<COMMENT>"*/"        { BEGIN(INITIAL); }
<COMMENT><<EOF>>      { printf("Erro lexico na linha %d: comentario de bloco nao fechado\n", yylineno); lexical_errors++; yyterminate(); }
<COMMENT>.           { /* Ignora caracteres dentro do comentario */ }
<COMMENT>\n          { /* Ignora quebras de linha dentro do comentario */ }

[ \t\r\n]+              { /* Ignora espacos em branco e quebras de linha */ }

 /* --- TIPOS DE DADOS --- */
"int"                   { return INT; }
"char"                  { return CHAR; }
"float"                 { return FLOAT; }
"double"                { return DOUBLE; }
"void"                  { return VOID; }

 /* --- CONDICIONAIS E REPETIÇÃO --- */
"if"                    { return IF; }
"else"                  { return ELSE; }
"while"                 { return WHILE; }
"for"                   { return FOR; }

 /* FUNÇÕES --- */
"return"                { return RETURN; }

 /* --- IDENTIFICADORES E NÚMEROS --- */

\'([^\\\'\n]|\\[ntr0\\\'\"])\' {
    char c;

    if (yytext[1] == '\\') {
        switch (yytext[2]) {
            case 'n': c = '\n'; break;
            case 't': c = '\t'; break;
            case 'r': c = '\r'; break;
            case '0': c = '\0'; break;
            case '\\': c = '\\'; break;
            case '\'': c = '\''; break;
            case '"': c = '"'; break;
            default: c = yytext[2]; break;
        }
    } else {
        c = yytext[1];
    }

    yylval.num = c;
    return CHAR_LITERAL;
}

[a-zA-Z_][a-zA-Z0-9_]* { yylval.str = copiar_texto(yytext); return ID; }

[0-9]+"."[0-9]+ {
    yylval.fnum = atof(yytext);
    return NUM_FLOAT;
}

[0-9]+ {
    yylval.num = atoi(yytext);
    return NUM;
}

 /* --- OPERADORES DE COMPARAÇÃO --- */
"=="                    { return EQ; }
"!="                    { return NEQ; }
">="                    { return GEQ; }
"<="                    { return LEQ; }
">"                     { return '>'; }
"<"                     { return '<'; }

 /* --- OPERADORES DE INCREMENTO E DECREMENTO --- */
"++"                    { return INC; }
"--"                    { return DEC; }

 /* --- OPERADORES LÓGICOS (Padrão C) --- */
"&&"                    { return AND; }
"||"                    { return OR; }
"!"                     { return NOT; }

 /* --- OPERADORES ARITMÉTICOS E ATRIBUIÇÃO --- */
"+"                     { return '+'; }
"-"                     { return '-'; }
"*"                     { return '*'; }
"/"                     { return '/'; }
"%"                     { return '%'; }
"="                     { return '='; }

 /* --- PONTUAÇÃO E DELIMITADORES --- */
"("                     { return '('; }
")"                     { return ')'; }
"{"                     { return '{'; }
"}"                     { return '}'; }
";"                     { return ';'; }
","                     { return ','; }
"["                     { return '['; }
"]"                     { return ']'; }

 /* --- TRATAMENTO DE ERROS --- */
.                       { printf("Erro lexico na linha %d: %s\n", yylineno, yytext); lexical_errors++; }

%%

int yywrap(void) {
    return 1;
}