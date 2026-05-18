#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h"
#include "parser.tab.h"

static const char *type_to_string(int data_type) {
    switch (data_type) {
        case INT: return "int";
        case CHAR: return "char";
        case FLOAT: return "float";
        default: return "desconhecido";
    }
}

static const char *kind_to_string(SymbolKind kind) {
    switch (kind) {
        case SYMBOL_VARIABLE: return "variavel";
        case SYMBOL_ARRAY: return "array";
        case SYMBOL_FUNCTION: return "funcao";
        case SYMBOL_PARAMETER: return "parametro";
        default: return "desconhecido";
    }
}

void init_symbol_table(SymbolTable *table) {
    table->head = NULL;
    table->current_scope = 0;
    table->semantic_errors = 0;
}

void enter_scope(SymbolTable *table) {
    table->current_scope++;
}

void exit_scope(SymbolTable *table) {
    if (table->current_scope > 0) {
        table->current_scope--;
    }
}

Symbol *lookup_symbol_in_current_scope(SymbolTable *table, const char *name) {
    for (Symbol *symbol = table->head; symbol != NULL; symbol = symbol->next) {
        if (symbol->scope == table->current_scope && strcmp(symbol->name, name) == 0) {
            return symbol;
        }
    }

    return NULL;
}

Symbol *lookup_symbol(SymbolTable *table, const char *name) {
    Symbol *best_match = NULL;

    for (Symbol *symbol = table->head; symbol != NULL; symbol = symbol->next) {
        if (strcmp(symbol->name, name) == 0 && symbol->scope <= table->current_scope) {
            if (best_match == NULL || symbol->scope > best_match->scope) {
                best_match = symbol;
            }
        }
    }

    return best_match;
}

int insert_symbol(SymbolTable *table, const char *name, int data_type,
                  SymbolKind kind, int array_size, int parameter_count) {
    if (lookup_symbol_in_current_scope(table, name) != NULL) {
        printf("Erro semantico: '%s' ja foi declarado neste escopo\n", name);
        table->semantic_errors++;
        return 0;
    }

    Symbol *symbol = malloc(sizeof(Symbol));
    if (symbol == NULL) {
        printf("Erro interno: falha ao alocar simbolo\n");
        exit(1);
    }

    symbol->name = strdup(name);
    symbol->data_type = data_type;
    symbol->kind = kind;
    symbol->scope = table->current_scope;
    symbol->array_size = array_size;
    symbol->parameter_count = parameter_count;
    symbol->next = table->head;
    table->head = symbol;

    return 1;
}

static int count_list(ASTNode *list) {
    int count = 0;

    while (list != NULL) {
        count++;
        list = list->list.next;
    }

    return count;
}

static int count_arguments(ASTNode *list) {
    return count_list(list);
}

static void require_symbol(SymbolTable *table, const char *name, const char *usage) {
    if (lookup_symbol(table, name) == NULL) {
        printf("Erro semantico: %s '%s' nao declarado(a)\n", usage, name);
        table->semantic_errors++;
    }
}

static void analyze_list_as_parameters(ASTNode *list, SymbolTable *table) {
    while (list != NULL) {
        ASTNode *item = list->list.item;

        if (item != NULL && item->type == AST_DECL) {
            insert_symbol(table, item->decl.var_name, item->decl.data_type,
                          SYMBOL_PARAMETER, 0, 0);
        }

        list = list->list.next;
    }
}

void analyze_semantics(ASTNode *node, SymbolTable *table) {
    if (node == NULL) return;

    switch (node->type) {
        case AST_NUM:
        case AST_FLOAT:
            break;

        case AST_ID:
            require_symbol(table, node->var_name, "variavel");
            break;

        case AST_BINOP:
            analyze_semantics(node->binop.left, table);
            analyze_semantics(node->binop.right, table);
            break;

        case AST_UNOP:
            analyze_semantics(node->unop.operand, table);
            break;

        case AST_ASSIGN:
            require_symbol(table, node->assign.var_name, "variavel");
            analyze_semantics(node->assign.value, table);
            break;

        case AST_ASSIGN_ARRAY:
            require_symbol(table, node->assign_array.var_name, "array");
            analyze_semantics(node->assign_array.index, table);
            analyze_semantics(node->assign_array.value, table);
            break;

        case AST_IF:
            analyze_semantics(node->if_stmt.condition, table);
            analyze_semantics(node->if_stmt.if_body, table);
            analyze_semantics(node->if_stmt.else_body, table);
            break;

        case AST_WHILE:
            analyze_semantics(node->while_stmt.condition, table);
            analyze_semantics(node->while_stmt.body, table);
            break;

        case AST_FOR:
            analyze_semantics(node->for_stmt.init, table);
            analyze_semantics(node->for_stmt.condition, table);
            analyze_semantics(node->for_stmt.update, table);
            analyze_semantics(node->for_stmt.body, table);
            break;

        case AST_SEQ:
            analyze_semantics(node->seq.first, table);
            analyze_semantics(node->seq.next, table);
            break;

        case AST_DECL:
            insert_symbol(table, node->decl.var_name, node->decl.data_type,
                          SYMBOL_VARIABLE, 0, 0);
            analyze_semantics(node->decl.value, table);
            break;

        case AST_ARRAY_DECL:
            insert_symbol(table, node->array_decl.var_name, node->array_decl.data_type,
                          SYMBOL_ARRAY, node->array_decl.size, 0);
            analyze_semantics(node->array_decl.values, table);
            break;

        case AST_FUNC_DECL:
            insert_symbol(table, node->func_decl.func_name, node->func_decl.return_type,
                          SYMBOL_FUNCTION, 0, count_list(node->func_decl.params));

            enter_scope(table);
            analyze_list_as_parameters(node->func_decl.params, table);
            analyze_semantics(node->func_decl.body, table);
            exit_scope(table);
            break;

        case AST_RETURN:
            analyze_semantics(node->return_stmt.expr, table);
            break;

        case AST_FUNC_CALL: {
            Symbol *symbol = lookup_symbol(table, node->func_call.func_name);

            if (symbol == NULL) {
                printf("Erro semantico: funcao '%s' nao declarado(a)\n", node->func_call.func_name);
                table->semantic_errors++;
            } else if (symbol->kind != SYMBOL_FUNCTION) {
                printf("Erro semantico: '%s' nao e uma funcao\n", node->func_call.func_name);
                table->semantic_errors++;
            } else if (symbol->parameter_count != count_arguments(node->func_call.args)) {
                printf("Erro semantico: funcao '%s' esperava %d argumento(s), mas recebeu %d\n",
                       node->func_call.func_name,
                       symbol->parameter_count,
                       count_arguments(node->func_call.args));
                table->semantic_errors++;
            }

            analyze_semantics(node->func_call.args, table);
            break;
        }

        case AST_ARRAY_ACCESS:
            require_symbol(table, node->array_access.var_name, "array");
            analyze_semantics(node->array_access.index, table);
            break;

        case AST_LIST:
            analyze_semantics(node->list.item, table);
            analyze_semantics(node->list.next, table);
            break;

        default:
            break;
    }
}

void print_symbol_table(SymbolTable *table) {
    printf("%-16s %-12s %-12s %-8s %-10s %-10s\n",
           "Nome", "Tipo", "Categoria", "Escopo", "TamArray", "Params");
    printf("----------------------------------------------------------------------\n");

    for (Symbol *symbol = table->head; symbol != NULL; symbol = symbol->next) {
        printf("%-16s %-12s %-12s %-8d %-10d %-10d\n",
               symbol->name,
               type_to_string(symbol->data_type),
               kind_to_string(symbol->kind),
               symbol->scope,
               symbol->array_size,
               symbol->parameter_count);
    }

    if (table->semantic_errors == 0) {
        printf("\nAnalise semantica concluida sem erros.\n");
    } else {
        printf("\nAnalise semantica encontrou %d erro(s).\n", table->semantic_errors);
    }
}
