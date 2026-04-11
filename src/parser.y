%{
#include <stdio.h>
#include <stdlib.h>

void yyerror(const char *s);
int yylex(void);
%}

/* --- Uniao para valores semanticos --- */
%union {
    int num;
    char* str;
}

/* --- Tokens vindos do lexer --- */
%token INT CHAR FLOAT
%token IF ELSE WHILE FOR
%token ID NUM NUM_FLOAT
%token EQ NEQ GEQ LEQ AND OR NOT

/* --- Tipagem dos tokens --- */
%type <num> NUM NUM_FLOAT
%type <str> ID

/* --- Precedencia de operadores --- */
%left OR
%left AND
%left EQ NEQ
%left '>' '<' GEQ LEQ
%left '+' '-'
%left '*' '/'
%right NOT

/* --- Correcao do dangling else --- */
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

/* --- Regra inicial --- */
program:
    statements
;

/* --- Lista de comandos --- */
statements:
      statements statement
    | statement
;

/* --- Tipos de comandos --- */
statement:
      declaration ';'
    | assignment ';'
    | if_statement
    | while_statement
    | for_statement
    | '{' statements '}'
;

/* --- Declaracao de variavel --- */
declaration:
      type ID
    | type ID '=' expression
;

/* --- Tipos --- */
type:
      INT
    | CHAR
    | FLOAT
;

/* --- Atribuicao --- */
assignment:
    ID '=' expression
;

/* --- If / Else --- */
if_statement:
      IF '(' expression ')' statement %prec LOWER_THAN_ELSE
    | IF '(' expression ')' statement ELSE statement
;

/* --- While --- */
while_statement:
    WHILE '(' expression ')' statement
;

/* --- For --- */
for_statement:
    FOR '(' assignment ';' expression ';' assignment ')' statement
;

/* --- Expressoes --- */
expression:
      expression '+' expression
    | expression '-' expression
    | expression '*' expression
    | expression '/' expression
    | expression EQ expression
    | expression NEQ expression
    | expression '>' expression
    | expression '<' expression
    | expression GEQ expression
    | expression LEQ expression
    | expression AND expression
    | expression OR expression
    | NOT expression
    | '(' expression ')'
    | NUM
    | NUM_FLOAT
    | ID
;

%%

/* --- Tratamento de erro --- */
void yyerror(const char *s) {
    printf("Erro sintatico: %s\n", s);
}

/* --- Funcao principal --- */
int main() {
    printf("Iniciando analise sintatica...\n");
    yyparse();
    printf("Analise concluida!\n");
    return 0;
}