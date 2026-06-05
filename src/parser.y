%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symtable.h"

void yyerror(const char *s);
int yylex(void);
extern int lexical_errors;

ASTNode* raiz_da_arvore;
int current_type;  /* Armazena o tipo atual para declaracoes multiplas */
%}

/* --- Uniao para valores semanticos --- */
%union {
    int num;
    float fnum;
    char* str;
    struct ASTNode* node;
    struct Declarator {
        char* name;
        struct ASTNode* init;
        int size;
        int is_array;
    } declarator;
}

/* --- Tokens vindos do lexer --- */
%token INT CHAR FLOAT DOUBLE VOID
%token IF ELSE WHILE FOR
%token <str> ID
%token <num> NUM CHAR_LITERAL
%token <fnum> NUM_FLOAT
%token EQ NEQ GEQ LEQ AND OR NOT INC DEC RETURN
%destructor { free($$); } <str>

/* --- Tipagem das regras (Não-terminais) --- */
%type <node> program global_declarations global_declaration statements optional_statements statement expression
%type <node> if_statement while_statement for_statement for_init for_condition for_update assignment
%type <node> param_list params param arg_list args function_declaration function_prototype
%type <node> declaration declarator_list for_declaration
%type <num> type
%type <declarator> declarator

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
    global_declarations { raiz_da_arvore = $1; }
;

/* --- Declaracoes no nivel global (arquivo) --- */
global_declarations:
      /* vazio */                       { $$ = NULL; }
    | global_declarations global_declaration { $$ = criar_no_bloco($1, $2); }
;

global_declaration:
      function_declaration              { $$ = $1; }
    | function_prototype ';'            { $$ = $1; }
    | declaration ';'                   { $$ = $1; }
;

/* --- Lista de comandos (dentro de funcoes/blocos) --- */
optional_statements:
      /* vazio */           { $$ = NULL; }
    | statements            { $$ = $1; }
;

statements:
      statements statement  { $$ = criar_no_bloco($1, $2); }
    | statement             { $$ = $1; }
;

/* --- Listas de Parametros (Para criar a funcao) --- */
param_list:
      /* vazio */           { $$ = NULL; }
    | VOID                  { $$ = NULL; }
    | params                { $$ = $1; }
;

params:
      param                 { $$ = criar_no_lista($1); }
    | params ',' param      { $$ = adicionar_na_lista($1, $3); }
;

param:
    type ID                 { $$ = criar_no_decl($1, $2, NULL); free($2); }
;

/* --- Prototipo de Funcao --- */
function_prototype:
    type ID '(' param_list ')' { $$ = criar_no_func_decl($1, $2, $4, NULL); free($2); }
;

/* --- Definicao de Funcao (com corpo) --- */
function_declaration:
    type ID '(' param_list ')' '{' optional_statements '}' { $$ = criar_no_func_decl($1, $2, $4, $7); free($2); }
;

/* --- Declaracao de variavel (com suporte a multiplas variaveis) --- */
declaration:
      type declarator_list { $$ = $2; }
;

declarator_list:
      declarator                       { 
          if ($1.is_array) {
              $$ = criar_no_lista(criar_no_array_decl(current_type, $1.name, $1.size, $1.init));
          } else {
              $$ = criar_no_lista(criar_no_decl(current_type, $1.name, $1.init));
          }
          free($1.name); 
      }
    | declarator_list ',' declarator   { 
          if ($3.is_array) {
              $$ = adicionar_na_lista($1, criar_no_array_decl(current_type, $3.name, $3.size, $3.init));
          } else {
              $$ = adicionar_na_lista($1, criar_no_decl(current_type, $3.name, $3.init));
          }
          free($3.name); 
      }
;

declarator:
      ID                               { $$.name = $1; $$.init = NULL; $$.size = 0; $$.is_array = 0; }
    | ID '=' expression                { $$.name = $1; $$.init = $3; $$.size = 0; $$.is_array = 0; }
    | ID '[' NUM ']'                   { $$.name = $1; $$.init = NULL; $$.size = $3; $$.is_array = 1; }
    | ID '[' NUM ']' '=' '{' arg_list '}' { $$.name = $1; $$.init = $7; $$.size = $3; $$.is_array = 1; }
    | ID '[' ']' '=' '{' arg_list '}'  { $$.name = $1; $$.init = $6; $$.size = -1; $$.is_array = 1; }
;

/* --- Declaracao especifica para o for (cria um no de bloco) --- */
for_declaration:
      declaration { 
          /* Para o for, precisamos garantir que arrays tambem sejam tratados corretamente */
          $$ = criar_no_escopo($1); 
      }
;

/* --- Tipos de comandos (NÃO inclui declaracao de funcao) --- */
statement:
      declaration ';'           { $$ = $1; }
    | assignment ';'            { $$ = $1; }
    | expression ';'            { $$ = $1; }
    | if_statement              { $$ = $1; }
    | while_statement           { $$ = $1; }
    | for_statement             { $$ = $1; }
    | '{' optional_statements '}' { $$ = criar_no_escopo($2); }
    | RETURN expression ';'     { $$ = criar_no_return($2); }
    | RETURN ';'                { $$ = criar_no_return(NULL); }
;

/* --- Tipos --- */
type:
      INT   { current_type = INT; $$ = INT; }
    | CHAR  { current_type = CHAR; $$ = CHAR; }
    | FLOAT { current_type = FLOAT; $$ = FLOAT; }
    | DOUBLE { current_type = DOUBLE; $$ = DOUBLE; }
    | VOID  { current_type = VOID; $$ = VOID; }
;

/* --- Atribuicao --- */
assignment:
      ID '=' expression                     { $$ = criar_no_atribuicao($1, $3); free($1); }
    | ID '[' expression ']' '=' expression  { $$ = criar_no_atribuicao_array($1, $3, $6); free($1); }
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

/* --- For --- */
for_init:
      for_declaration      { $$ = $1; }
    | assignment           { $$ = $1; }
    | expression           { $$ = $1; }
    | /* vazio */          { $$ = NULL; }
;

for_condition:
      expression           { $$ = $1; }
    | /* vazio */          { $$ = NULL; }
;

for_update:
      assignment           { $$ = $1; }
    | expression           { $$ = $1; }
    | /* vazio */          { $$ = NULL; }
;

for_statement:
    FOR '(' for_init ';' for_condition ';' for_update ')' statement 
    { 
        ASTNode* result;
        result = criar_no_for($3, $5, $7, $9);
        $$ = result;
    }
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
    | ID '(' arg_list ')'       { $$ = criar_no_func_call($1, $3); free($1); }
    | ID '[' expression ']'     { $$ = criar_no_array_access($1, $3); free($1); }
    | NUM                       { $$ = criar_no_numero($1); }
    | NUM_FLOAT                 { $$ = criar_no_float($1); }
    | CHAR_LITERAL              { $$ = criar_no_char_literal((char)$1); }
    | ID                        { $$ = criar_no_id($1); free($1); }
;

%%

/* --- Tratamento de erro --- */
void yyerror(const char *s) {
    printf("Erro sintatico: %s\n", s);
}

/* --- Funcao principal --- */
int main() {
    printf("Iniciando analise sintatica...\n");

    int parse_result = yyparse();

    if (parse_result == 0 && lexical_errors == 0) {
        printf("Analise concluida com sucesso!\n");
        printf("\n--- TABELA DE SIMBOLOS ---\n");
        SymbolTable tabela;
        init_symbol_table(&tabela);
        analyze_semantics(raiz_da_arvore, &tabela);
        print_symbol_table(&tabela);

        printf("\n--- ARVORE SINTATICA GERADA ---\n");
        imprimir_ast(raiz_da_arvore, 0);

        free_symbol_table(&tabela);
        liberar_ast(raiz_da_arvore);
    } else if (lexical_errors > 0) {
        printf("A analise falhou devido a erros lexicos.\n");
    } else {
        printf("A analise falhou devido a erros sintaticos.\n");
    }

    return 0;
}