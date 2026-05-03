/* arquivo: ast.h */
#ifndef AST_H
#define AST_H

/* --- 1. Enumerador de Tipos de Nós --- */
typedef enum {
    AST_NUM,            /* Número inteiro */
    AST_FLOAT,          /* Número quebrado */
    AST_ID,             /* Nome de variável */
    AST_BINOP,          /* Operação Binária (+, -, *, /) */
    AST_UNOP,           /* Operação Unária (-x, !x, x++) */
    AST_ASSIGN,         /* Atribuição simples (x = 5) */
    AST_ASSIGN_ARRAY,   /* Atribuição em array (x[0] = 5) */
    AST_IF,             /* Estrutura Condicional If/Else */
    AST_WHILE,          /* Laço While */
    AST_FOR,            /* Laço For */
    AST_SEQ,            /* Sequência de comandos (Bloco) */
    AST_DECL,           /* Declaração de variável (int x = 5) */
    AST_ARRAY_DECL,     /* Declaração de array (int x[10]) */
    AST_FUNC_DECL,      /* Declaração de função */
    AST_RETURN,         /* Comando Return */
    AST_FUNC_CALL,      /* Chamada de função ( soma(a, b) ) */
    AST_ARRAY_ACCESS,   /* Acesso a índice de array ( x[2] ) */
    AST_LIST            /* Lista genérica (para parâmetros e argumentos) */
} NodeType;

/* --- 2. Estrutura do Nó (A Caixinha Universal) --- */
typedef struct ASTNode ASTNode;

struct ASTNode {
    NodeType type;
    
    union {
        int int_val;       
        float float_val;   
        char* var_name;    
        
        struct { int operator; ASTNode* left; ASTNode* right; } binop;
        struct { int operator; ASTNode* operand; } unop;
        struct { ASTNode* condition; ASTNode* if_body; ASTNode* else_body; } if_stmt;
        struct { ASTNode* condition; ASTNode* body; } while_stmt;
        struct { ASTNode* init; ASTNode* condition; ASTNode* update; ASTNode* body; } for_stmt;
        struct { ASTNode* first; ASTNode* next; } seq;
        
        struct { char* var_name; ASTNode* value; } assign;
        struct { char* var_name; ASTNode* index; ASTNode* value; } assign_array;
        
        struct { int data_type; char* var_name; ASTNode* value; } decl;
        struct { int data_type; char* var_name; int size; ASTNode* values; } array_decl;
        struct { int return_type; char* func_name; ASTNode* params; ASTNode* body; } func_decl;
        
        struct { ASTNode* expr; } return_stmt;
        struct { char* func_name; ASTNode* args; } func_call;
        struct { char* var_name; ASTNode* index; } array_access;
        
        struct { ASTNode* item; ASTNode* next; } list;
    };
};

/* --- 3. Assinaturas das Funções Construtoras --- */
ASTNode* criar_no_numero(int valor);
ASTNode* criar_no_float(float valor);
ASTNode* criar_no_id(char* nome);
ASTNode* criar_no_binop(int operador, ASTNode* esq, ASTNode* dir);
ASTNode* criar_no_unop(int operador, ASTNode* operando);
ASTNode* criar_no_bloco(ASTNode* comando_atual, ASTNode* proximo_comando);
ASTNode* criar_no_if(ASTNode* condicao, ASTNode* corpo_if, ASTNode* corpo_else);
ASTNode* criar_no_while(ASTNode* condicao, ASTNode* corpo);
ASTNode* criar_no_for(ASTNode* init, ASTNode* condicao, ASTNode* update, ASTNode* corpo);
ASTNode* criar_no_atribuicao(char* nome, ASTNode* valor);
ASTNode* criar_no_atribuicao_array(char* nome, ASTNode* indice, ASTNode* valor);
ASTNode* criar_no_decl(int tipo, char* nome, ASTNode* valor);
ASTNode* criar_no_array_decl(int tipo, char* nome, int tamanho, ASTNode* valores);
ASTNode* criar_no_func_decl(int tipo_retorno, char* nome, ASTNode* parametros, ASTNode* corpo);
ASTNode* criar_no_return(ASTNode* expressao);
ASTNode* criar_no_func_call(char* nome, ASTNode* argumentos);
ASTNode* criar_no_array_access(char* nome, ASTNode* indice);
ASTNode* criar_no_lista(ASTNode* item);
ASTNode* adicionar_na_lista(ASTNode* lista, ASTNode* novo_item);

void imprimir_ast(ASTNode* no, int nivel);

#endif