/* arquivo: ast.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "parser.tab.h" /* Necessário para reconhecer os tokens como IF, WHILE, INT */

static ASTNode* novo_no(void) {
    ASTNode* no = malloc(sizeof(ASTNode));
    if (no == NULL) {
        fprintf(stderr, "Erro interno: falha ao alocar no da AST\n");
        exit(1);
    }
    return no;
}

static char* copiar_string(const char* texto) {
    if (texto == NULL) return NULL;

    char* copia = malloc(strlen(texto) + 1);
    if (copia == NULL) {
        fprintf(stderr, "Erro interno: falha ao copiar texto da AST\n");
        exit(1);
    }

    strcpy(copia, texto);
    return copia;
}

/* --- Funções de Alocação Básicas --- */
ASTNode* criar_no_numero(int valor) {
    ASTNode* no = novo_no();
    no->type = AST_NUM;
    no->int_val = valor;
    return no;
}

ASTNode* criar_no_float(float valor) {
    ASTNode* no = novo_no();
    no->type = AST_FLOAT;
    no->float_val = valor;
    return no;
}

ASTNode* criar_no_char_literal(char valor) {
    ASTNode* no = novo_no();
    no->type = AST_CHAR_LITERAL;
    no->char_val = valor;
    return no;
}

ASTNode* criar_no_id(char* nome) {
    ASTNode* no = novo_no();
    no->type = AST_ID;
    no->var_name = copiar_string(nome);
    return no;
}

/* --- Expressões --- */
ASTNode* criar_no_binop(int operador, ASTNode* esq, ASTNode* dir) {
    ASTNode* no = novo_no();
    no->type = AST_BINOP;
    no->binop.operator = operador;
    no->binop.left = esq;
    no->binop.right = dir;
    return no;
}

ASTNode* criar_no_unop(int operador, ASTNode* operando) {
    ASTNode* no = novo_no();
    no->type = AST_UNOP;
    no->unop.operator = operador;
    no->unop.operand = operando;
    return no;
}

ASTNode* criar_no_func_call(char* nome, ASTNode* argumentos) {
    ASTNode* no = novo_no();
    no->type = AST_FUNC_CALL;
    no->func_call.func_name = copiar_string(nome);
    no->func_call.args = argumentos;
    return no;
}

ASTNode* criar_no_array_access(char* nome, ASTNode* indice) {
    ASTNode* no = novo_no();
    no->type = AST_ARRAY_ACCESS;
    no->array_access.var_name = copiar_string(nome);
    no->array_access.index = indice;
    return no;
}

/* --- Controle de Fluxo e Blocos --- */
ASTNode* criar_no_bloco(ASTNode* comando_atual, ASTNode* proximo_comando) {
    ASTNode* no = novo_no();
    no->type = AST_SEQ;
    no->seq.first = comando_atual;
    no->seq.next = proximo_comando;
    return no;
}

ASTNode* criar_no_escopo(ASTNode* comandos) {
    ASTNode* no = novo_no();
    no->type = AST_BLOCK;
    no->block.statements = comandos;
    return no;
}

ASTNode* criar_no_if(ASTNode* condicao, ASTNode* corpo_if, ASTNode* corpo_else) {
    ASTNode* no = novo_no();
    no->type = AST_IF;
    no->if_stmt.condition = condicao;
    no->if_stmt.if_body = corpo_if;
    no->if_stmt.else_body = corpo_else;
    return no;
}

ASTNode* criar_no_while(ASTNode* condicao, ASTNode* corpo) {
    ASTNode* no = novo_no();
    no->type = AST_WHILE;
    no->while_stmt.condition = condicao;
    no->while_stmt.body = corpo;
    return no;
}

ASTNode* criar_no_for(ASTNode* init, ASTNode* condicao, ASTNode* update, ASTNode* corpo) {
    ASTNode* no = novo_no();
    no->type = AST_FOR;
    no->for_stmt.init = init;
    no->for_stmt.condition = condicao;
    no->for_stmt.update = update;
    no->for_stmt.body = corpo;
    return no;
}

/* --- Declarações e Atribuições --- */
ASTNode* criar_no_atribuicao(char* nome, ASTNode* valor) {
    ASTNode* no = novo_no();
    no->type = AST_ASSIGN;
    no->assign.var_name = copiar_string(nome);
    no->assign.value = valor;
    return no;
}

ASTNode* criar_no_atribuicao_array(char* nome, ASTNode* indice, ASTNode* valor) {
    ASTNode* no = novo_no();
    no->type = AST_ASSIGN_ARRAY;
    no->assign_array.var_name = copiar_string(nome);
    no->assign_array.index = indice;
    no->assign_array.value = valor;
    return no;
}

ASTNode* criar_no_decl(int tipo, char* nome, ASTNode* valor) {
    ASTNode* no = novo_no();
    no->type = AST_DECL;
    no->decl.data_type = tipo;
    no->decl.var_name = copiar_string(nome);
    no->decl.value = valor;
    return no;
}

ASTNode* criar_no_array_decl(int tipo, char* nome, int tamanho, ASTNode* valores) {
    ASTNode* no = novo_no();
    no->type = AST_ARRAY_DECL;
    no->array_decl.data_type = tipo;
    no->array_decl.var_name = copiar_string(nome);
    no->array_decl.size = tamanho;
    no->array_decl.values = valores;
    return no;
}

ASTNode* criar_no_func_decl(int tipo_retorno, char* nome, ASTNode* parametros, ASTNode* corpo) {
    ASTNode* no = novo_no();
    no->type = AST_FUNC_DECL;
    no->func_decl.return_type = tipo_retorno;
    no->func_decl.func_name = copiar_string(nome);
    no->func_decl.params = parametros;
    no->func_decl.body = corpo;
    return no;
}

ASTNode* criar_no_return(ASTNode* expressao) {
    ASTNode* no = novo_no();
    no->type = AST_RETURN;
    no->return_stmt.expr = expressao;
    return no;
}

/* --- Listas (Parâmetros e Argumentos) --- */
ASTNode* criar_no_lista(ASTNode* item) {
    ASTNode* no = novo_no();
    no->type = AST_LIST;
    no->list.item = item;
    no->list.next = NULL;
    return no;
}

ASTNode* adicionar_na_lista(ASTNode* lista, ASTNode* novo_item) {
    if (lista == NULL) return criar_no_lista(novo_item);
    ASTNode* atual = lista;
    while (atual->list.next != NULL) {
        atual = atual->list.next;
    }
    atual->list.next = criar_no_lista(novo_item);
    return lista;
}

/* --- Ferramenta de Debug Visual --- */
void imprimir_ast(ASTNode* no, int nivel) {
    if (no == NULL) return;

    for (int i = 0; i < nivel; i++) printf("  ");

    switch (no->type) {
        case AST_NUM: printf("NUMERO: %d\n", no->int_val); break;
        case AST_FLOAT: printf("FLOAT: %f\n", no->float_val); break;
        case AST_CHAR_LITERAL: printf("CHAR: '%c'\n", no->char_val); break;
        case AST_ID: printf("VARIAVEL: %s\n", no->var_name); break;
        case AST_BINOP:
            printf("BINOP: ");
            if (no->binop.operator >= 256) printf("[TOKEN %d]\n", no->binop.operator);
            else printf("%c\n", no->binop.operator);
            imprimir_ast(no->binop.left, nivel + 1);
            imprimir_ast(no->binop.right, nivel + 1);
            break;
        case AST_UNOP:
            printf("UNOP: ");
            if (no->unop.operator >= 256) printf("[TOKEN %d]\n", no->unop.operator);
            else printf("%c\n", no->unop.operator);
            imprimir_ast(no->unop.operand, nivel + 1);
            break;
        case AST_SEQ:
            printf("--- COMANDO ---\n");
            imprimir_ast(no->seq.first, nivel + 1);
            imprimir_ast(no->seq.next, nivel);
            break;
        case AST_BLOCK:
            imprimir_ast(no->block.statements, nivel);
            break;
        case AST_IF:
            printf("IF\n");
            imprimir_ast(no->if_stmt.condition, nivel + 1);
            printf("THEN\n");
            imprimir_ast(no->if_stmt.if_body, nivel + 1);
            if (no->if_stmt.else_body != NULL) {
                for (int i = 0; i < nivel; i++) printf("  ");
                printf("ELSE\n");
                imprimir_ast(no->if_stmt.else_body, nivel + 1);
            }
            break;
        case AST_WHILE:
            printf("WHILE\n");
            imprimir_ast(no->while_stmt.condition, nivel + 1);
            printf("DO\n");
            imprimir_ast(no->while_stmt.body, nivel + 1);
            break;
        case AST_FOR:
            printf("FOR\n");
            imprimir_ast(no->for_stmt.init, nivel + 1);
            imprimir_ast(no->for_stmt.condition, nivel + 1);
            imprimir_ast(no->for_stmt.update, nivel + 1);
            printf("DO\n");
            imprimir_ast(no->for_stmt.body, nivel + 1);
            break;
        case AST_ASSIGN:
            printf("ATRIBUICAO: %s =\n", no->assign.var_name);
            imprimir_ast(no->assign.value, nivel + 1);
            break;
        case AST_ASSIGN_ARRAY:
            printf("ATRIBUICAO ARRAY: %s[] =\n", no->assign_array.var_name);
            imprimir_ast(no->assign_array.index, nivel + 1);
            imprimir_ast(no->assign_array.value, nivel + 1);
            break;
        case AST_DECL:
            printf("DECLARACAO: tipo[%d] %s\n", no->decl.data_type, no->decl.var_name);
            if (no->decl.value) imprimir_ast(no->decl.value, nivel + 1);
            break;
        case AST_ARRAY_DECL:
            printf("DECLARACAO ARRAY: tipo[%d] %s[%d]\n", no->array_decl.data_type, no->array_decl.var_name, no->array_decl.size);
            if (no->array_decl.values) imprimir_ast(no->array_decl.values, nivel + 1);
            break;
        case AST_FUNC_DECL:
            printf("FUNCAO DECLARADA: %s\n", no->func_decl.func_name);
            printf("PARAMETROS:\n");
            imprimir_ast(no->func_decl.params, nivel + 1);
            printf("CORPO:\n");
            imprimir_ast(no->func_decl.body, nivel + 1);
            break;
        case AST_RETURN:
            printf("RETURN\n");
            if (no->return_stmt.expr) imprimir_ast(no->return_stmt.expr, nivel + 1);
            break;
        case AST_FUNC_CALL:
            printf("CHAMADA DE FUNCAO: %s()\n", no->func_call.func_name);
            imprimir_ast(no->func_call.args, nivel + 1);
            break;
        case AST_ARRAY_ACCESS:
            printf("ACESSO A ARRAY: %s[]\n", no->array_access.var_name);
            imprimir_ast(no->array_access.index, nivel + 1);
            break;
        case AST_LIST:
            imprimir_ast(no->list.item, nivel);
            if (no->list.next) imprimir_ast(no->list.next, nivel);
            break;
        default:
            printf("Nó Desconhecido!\n");
    }
}

void liberar_ast(ASTNode* no) {
    if (no == NULL) return;

    switch (no->type) {
        case AST_NUM:
        case AST_FLOAT:
        case AST_CHAR_LITERAL:
            break;

        case AST_ID:
            free(no->var_name);
            break;

        case AST_BINOP:
            liberar_ast(no->binop.left);
            liberar_ast(no->binop.right);
            break;

        case AST_UNOP:
            liberar_ast(no->unop.operand);
            break;

        case AST_ASSIGN:
            free(no->assign.var_name);
            liberar_ast(no->assign.value);
            break;

        case AST_ASSIGN_ARRAY:
            free(no->assign_array.var_name);
            liberar_ast(no->assign_array.index);
            liberar_ast(no->assign_array.value);
            break;

        case AST_IF:
            liberar_ast(no->if_stmt.condition);
            liberar_ast(no->if_stmt.if_body);
            liberar_ast(no->if_stmt.else_body);
            break;

        case AST_WHILE:
            liberar_ast(no->while_stmt.condition);
            liberar_ast(no->while_stmt.body);
            break;

        case AST_FOR:
            liberar_ast(no->for_stmt.init);
            liberar_ast(no->for_stmt.condition);
            liberar_ast(no->for_stmt.update);
            liberar_ast(no->for_stmt.body);
            break;

        case AST_SEQ:
            liberar_ast(no->seq.first);
            liberar_ast(no->seq.next);
            break;

        case AST_BLOCK:
            liberar_ast(no->block.statements);
            break;

        case AST_DECL:
            free(no->decl.var_name);
            liberar_ast(no->decl.value);
            break;

        case AST_ARRAY_DECL:
            free(no->array_decl.var_name);
            liberar_ast(no->array_decl.values);
            break;

        case AST_FUNC_DECL:
            free(no->func_decl.func_name);
            liberar_ast(no->func_decl.params);
            liberar_ast(no->func_decl.body);
            break;

        case AST_RETURN:
            liberar_ast(no->return_stmt.expr);
            break;

        case AST_FUNC_CALL:
            free(no->func_call.func_name);
            liberar_ast(no->func_call.args);
            break;

        case AST_ARRAY_ACCESS:
            free(no->array_access.var_name);
            liberar_ast(no->array_access.index);
            break;

        case AST_LIST:
            liberar_ast(no->list.item);
            liberar_ast(no->list.next);
            break;
    }

    free(no);
}
