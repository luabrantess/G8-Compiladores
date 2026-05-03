%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

void yyerror(const char *s);
int yylex(void);

ASTNode* raiz_da_arvore;
%}

/* --- Uniao para valores semanticos --- */
%union {
    int num;
    char* str;
    struct ASTNode* node;
}

/* --- Tokens vindos do lexer --- */
%token INT CHAR FLOAT
%token IF ELSE WHILE FOR
%token ID NUM NUM_FLOAT
%token EQ NEQ GEQ LEQ AND OR NOT INC DEC RETURN

/* --- Tipagem das regras (Não-terminais) --- */
%type <node> program statements statement expression
%type <node> if_statement while_statement for_statement for_action assignment declaration
%type <node> param_list params param arg_list args function_declaration

/* --- Tipagem dos tokens e tipos simples --- */
%type <num> NUM NUM_FLOAT type
%type <str> ID

/* --- Precedencia de operadores --- */
%left OR
%left AND
%left EQ NEQ
%left '>' '<' GEQ LEQ
%left '+' '-'
%left '*' '/'
%right NOT UMINUS
%right INC DEC

/* --- Correcao do dangling else --- */
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

/* --- Regra inicial --- */
program:
    statements { raiz_da_arvore = $1; }
;

/* --- Lista de comandos --- */
statements:
      statements statement  { $$ = criar_no_bloco($1, $2); }
    | statement             { $$ = $1; }
;

/* --- Listas de Parametros (Para criar a funcao) --- */
param_list:
      /* vazio */           { $$ = NULL; }
    | params                { $$ = $1; }
;

params:
      param                 { $$ = criar_no_lista($1); }
    | params ',' param      { $$ = adicionar_na_lista($1, $3); }
;

param:
    type ID                 { $$ = criar_no_decl($1, $2, NULL); }
;

/* --- Definicao de Funcao --- */
function_declaration:
    type ID '(' param_list ')' '{' statements '}' { $$ = criar_no_func_decl($1, $2, $4, $7); }
;

/* --- Tipos de comandos --- */
statement:
      declaration ';'           { $$ = $1; }
    | assignment ';'            { $$ = $1; }
    | expression ';'            { $$ = $1; }
    | if_statement              { $$ = $1; }
    | while_statement           { $$ = $1; }
    | for_statement             { $$ = $1; }
    | '{' statements '}'        { $$ = $2; }
    | function_declaration      { $$ = $1; }
    | RETURN expression ';'     { $$ = criar_no_return($2); }
    | RETURN ';'                { $$ = criar_no_return(NULL); }
;

/* --- Declaracao de variavel --- */
declaration:
      type ID                                      { $$ = criar_no_decl($1, $2, NULL); }
    | type ID '=' expression                       { $$ = criar_no_decl($1, $2, $4); }
    | type ID '[' NUM ']'                          { $$ = criar_no_array_decl($1, $2, $4, NULL); }
    | type ID '[' NUM ']' '=' '{' arg_list '}'     { $$ = criar_no_array_decl($1, $2, $4, $8); }
;

/* --- Tipos --- */
type:
      INT   { $$ = INT; }
    | CHAR  { $$ = CHAR; }
    | FLOAT { $$ = FLOAT; }
;

/* --- Atribuicao --- */
assignment:
      ID '=' expression                     { $$ = criar_no_atribuicao($1, $3); }
    | ID '[' expression ']' '=' expression  { $$ = criar_no_atribuicao_array($1, $3, $6); }
;

/* --- If / Else --- */
if_statement:
      IF '(' expression ')' statement %prec LOWER_THAN_ELSE { $$ = criar_no_if($3, $5, NULL); }
    | IF '(' expression ')' statement ELSE statement        { $$ = criar_no_if($3, $5, $7); }
;

/* --- While --- */
while_statement:
    WHILE '(' expression ')' statement      { $$ = criar_no_while($3, $5); }
;

/* Necessario para o for reconhecer operadores de incremento x++*/
for_action:
      assignment    { $$ = $1; }
    | expression    { $$ = $1; }
;

/* --- For --- */
for_statement:
    FOR '(' assignment ';' expression ';' for_action ')' statement { $$ = criar_no_for($3, $5, $7, $9); }
;

/* --- Argumentos (Para chamar a funcao) --- */
arg_list:
      /* vazio */           { $$ = NULL; }
    | args                  { $$ = $1; }
;

args:
      expression            { $$ = criar_no_lista($1); }
    | args ',' expression   { $$ = adicionar_na_lista($1, $3); }
;

/* --- Expressoes --- */
expression:
      expression '+' expression { $$ = criar_no_binop('+', $1, $3); }
    | expression '-' expression { $$ = criar_no_binop('-', $1, $3); }
    | '-' expression %prec UMINUS { $$ = criar_no_unop('-', $2); }
    | expression INC            { $$ = criar_no_unop(INC, $1); }
    | INC expression            { $$ = criar_no_unop(INC, $2); }
    | expression DEC            { $$ = criar_no_unop(DEC, $1); }
    | DEC expression            { $$ = criar_no_unop(DEC, $2); }
    | expression '*' expression { $$ = criar_no_binop('*', $1, $3); }
    | expression '/' expression { $$ = criar_no_binop('/', $1, $3); }
    | expression EQ expression  { $$ = criar_no_binop(EQ, $1, $3); }
    | expression NEQ expression { $$ = criar_no_binop(NEQ, $1, $3); }
    | expression '>' expression { $$ = criar_no_binop('>', $1, $3); }
    | expression '<' expression { $$ = criar_no_binop('<', $1, $3); }
    | expression GEQ expression { $$ = criar_no_binop(GEQ, $1, $3); }
    | expression LEQ expression { $$ = criar_no_binop(LEQ, $1, $3); }
    | expression AND expression { $$ = criar_no_binop(AND, $1, $3); }
    | expression OR expression  { $$ = criar_no_binop(OR, $1, $3); }
    | NOT expression            { $$ = criar_no_unop(NOT, $2); }
    | '(' expression ')'        { $$ = $2; }
    | ID '(' arg_list ')'       { $$ = criar_no_func_call($1, $3); }
    | ID '[' expression ']'     { $$ = criar_no_array_access($1, $3); }
    | NUM                       { $$ = criar_no_numero($1); }
    | NUM_FLOAT                 { $$ = criar_no_float(0.0); /* Placeholder, já que o lexer atual não envia o float por yylval */ }
    | ID                        { $$ = criar_no_id($1); }
;

%%

/* --- Tratamento de erro --- */
void yyerror(const char *s) {
    printf("Erro sintatico: %s\n", s);
}

/* --- Funcao principal --- */
int main() {
    printf("Iniciando analise sintatica...\n");
    
    if (yyparse() == 0) {
        printf("Analise concluida com sucesso!\n");
        printf("\n--- ARVORE SINTATICA GERADA ---\n");
        imprimir_ast(raiz_da_arvore, 0);
    } else {
        printf("A analise falhou devido a erros sintaticos.\n");
    }
    
    return 0;
}