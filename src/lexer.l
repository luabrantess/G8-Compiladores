%{
#include "parser.tab.h"
#include <string.h>
#include <stdlib.h>
%}

%%

"//".* { /* Ignora comentarios de uma linha */ }
[ \t\n]+                { /* Ignora espacos em branco e quebras de linha */ }

 /* --- TIPOS DE DADOS --- */
"int"                   { return INT; }
"char"                  { return CHAR; }
"float"                 { return FLOAT; }

 /* --- CONDICIONAIS E REPETIÇÃO --- */
"if"                    { return IF; }
"else"                  { return ELSE; }
"while"                 { return WHILE; }
"for"                   { return FOR; }

 /* FUNÇÕES --- */
"return"                { return RETURN; }

 /* --- IDENTIFICADORES E NÚMEROS --- */
[a-zA-Z_][a-zA-Z0-9_]* { yylval.str = strdup(yytext); return ID; }
[0-9]+                  { yylval.num = atoi(yytext); return NUM; }
[0-9]+"."[0-9]+         { return NUM_FLOAT; }

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
.                       { printf("Erro lexico: %s\n", yytext); }

%%

int yywrap(void) {
    return 1;
}