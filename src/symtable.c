#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "symtable.h"
#include "parser.tab.h"

#define TYPE_UNKNOWN (-1)

typedef struct {
    SymbolTable *table;
    int inside_function;
    int current_function_return_type;
    const char *current_function_name;
    int semantic_errors;
} SemanticContext;

static void analyze_node(ASTNode *node, SemanticContext *ctx);
static int infer_expression_type(ASTNode *node, SemanticContext *ctx);

static void report_semantic_error(SemanticContext *ctx, const char *format, ...) {
    va_list args;

    printf("Erro semantico: ");
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("\n");

    ctx->semantic_errors++;
}

static const char *type_to_string(int data_type) {
    switch (data_type) {
        case INT: return "int";
        case CHAR: return "char";
        case FLOAT: return "float";
        case DOUBLE: return "double";
        case VOID: return "void";
        case TYPE_UNKNOWN: return "desconhecido";
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

static const char *operator_to_string(int operator) {
    switch (operator) {
        case '+': return "+";
        case '-': return "-";
        case '*': return "*";
        case '/': return "/";
        case '>': return ">";
        case '<': return "<";
        case EQ: return "==";
        case NEQ: return "!=";
        case GEQ: return ">=";
        case LEQ: return "<=";
        case AND: return "&&";
        case OR: return "||";
        case NOT: return "!";
        case INC: return "++";
        case DEC: return "--";
        default: return "operador desconhecido";
    }
}

static char *copy_string(const char *text) {
    char *copy = malloc(strlen(text) + 1);
    if (copy == NULL) {
        fprintf(stderr, "Erro interno: falha ao copiar nome de simbolo\n");
        exit(1);
    }

    strcpy(copy, text);
    return copy;
}

void init_symbol_table(SymbolTable *table) {
    table->head = NULL;
    table->current_scope = 0;
    table->next_scope = 1;
    table->scope_stack[0] = 0;
    table->scope_stack_top = 0;
    table->semantic_errors = 0;
}

void enter_scope(SymbolTable *table) {
    if (table->scope_stack_top >= 255) {
        fprintf(stderr, "Erro interno: limite de escopos excedido\n");
        exit(1);
    }

    table->current_scope = table->next_scope++;
    table->scope_stack[++table->scope_stack_top] = table->current_scope;
}

void exit_scope(SymbolTable *table) {
    if (table->scope_stack_top > 0) {
        table->scope_stack_top--;
        table->current_scope = table->scope_stack[table->scope_stack_top];
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
    for (int i = table->scope_stack_top; i >= 0; i--) {
        int active_scope = table->scope_stack[i];

        for (Symbol *symbol = table->head; symbol != NULL; symbol = symbol->next) {
            if (symbol->scope == active_scope && strcmp(symbol->name, name) == 0) {
                return symbol;
            }
        }
    }

    return NULL;
}

static Symbol *add_symbol(SymbolTable *table, const char *name, int data_type,
                          SymbolKind kind, int array_size, int parameter_count) {
    Symbol *symbol = malloc(sizeof(Symbol));
    if (symbol == NULL) {
        fprintf(stderr, "Erro interno: falha ao alocar simbolo\n");
        exit(1);
    }

    symbol->name = copy_string(name);
    symbol->data_type = data_type;
    symbol->kind = kind;
    symbol->scope = table->current_scope;
    symbol->array_size = array_size;
    symbol->parameter_count = parameter_count;
    symbol->parameter_types = NULL;
    symbol->next = table->head;
    table->head = symbol;

    return symbol;
}

int insert_symbol(SymbolTable *table, const char *name, int data_type,
                  SymbolKind kind, int array_size, int parameter_count) {
    if (lookup_symbol_in_current_scope(table, name) != NULL) {
        printf("Erro semantico: '%s' ja foi declarado neste escopo\n", name);
        table->semantic_errors++;
        return 0;
    }

    add_symbol(table, name, data_type, kind, array_size, parameter_count);
    return 1;
}

static Symbol *declare_symbol(SemanticContext *ctx, const char *name, int data_type,
                              SymbolKind kind, int array_size, int parameter_count) {
    /* Para funcoes, verificar no escopo global (scope 0) */
    if (kind == SYMBOL_FUNCTION) {
        for (Symbol *symbol = ctx->table->head; symbol != NULL; symbol = symbol->next) {
            if (symbol->scope == 0 && strcmp(symbol->name, name) == 0) {
                if (symbol->kind == SYMBOL_FUNCTION) {
                    return symbol; /* Retorna o simbolo existente */
                } else {
                    report_semantic_error(ctx, "'%s' ja foi declarado como %s, nao pode ser funcao", 
                                         name, kind_to_string(symbol->kind));
                    return NULL;
                }
            }
        }
    } else if (lookup_symbol_in_current_scope(ctx->table, name) != NULL) {
        report_semantic_error(ctx, "'%s' ja foi declarado neste escopo", name);
        return NULL;
    }

    return add_symbol(ctx->table, name, data_type, kind, array_size, parameter_count);
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

static int is_numeric_type(int data_type) {
    return data_type == INT || data_type == CHAR || data_type == FLOAT || data_type == DOUBLE;
}

static int is_void_type(int data_type) {
    return data_type == VOID;
}

static int is_type_compatible(int expected_type, int actual_type) {
    if (expected_type == TYPE_UNKNOWN || actual_type == TYPE_UNKNOWN) {
        return 1;
    }

    if (expected_type == actual_type) {
        return 1;
    }

    if (expected_type == DOUBLE && (actual_type == FLOAT || actual_type == INT || actual_type == CHAR)) {
        return 1;
    }

    if (expected_type == FLOAT && (actual_type == INT || actual_type == CHAR)) {
        return 1;
    }

    if (expected_type == INT && actual_type == CHAR) {
        return 1;
    }

    return 0;
}

static int arithmetic_result_type(int left_type, int right_type) {
    if (left_type == TYPE_UNKNOWN || right_type == TYPE_UNKNOWN) {
        return TYPE_UNKNOWN;
    }

    if (!is_numeric_type(left_type) || !is_numeric_type(right_type)) {
        return TYPE_UNKNOWN;
    }

    if (left_type == DOUBLE || right_type == DOUBLE) {
        return DOUBLE;
    }

    if (left_type == FLOAT || right_type == FLOAT) {
        return FLOAT;
    }

    return INT;
}

static Symbol *require_declared_symbol(SemanticContext *ctx, const char *name, const char *usage) {
    Symbol *symbol = lookup_symbol(ctx->table, name);

    if (symbol == NULL) {
        report_semantic_error(ctx, "%s '%s' nao declarado(a)", usage, name);
    }

    return symbol;
}

static Symbol *require_variable(SemanticContext *ctx, const char *name) {
    Symbol *symbol = require_declared_symbol(ctx, name, "variavel");

    if (symbol != NULL && symbol->kind != SYMBOL_VARIABLE && symbol->kind != SYMBOL_PARAMETER) {
        report_semantic_error(ctx, "'%s' nao e uma variavel", name);
        return NULL;
    }

    return symbol;
}

static Symbol *require_array(SemanticContext *ctx, const char *name) {
    Symbol *symbol = require_declared_symbol(ctx, name, "array");

    if (symbol != NULL && symbol->kind != SYMBOL_ARRAY) {
        report_semantic_error(ctx, "'%s' nao e um array", name);
        return NULL;
    }

    return symbol;
}

static int *copy_parameter_types(ASTNode *params, int parameter_count) {
    if (parameter_count <= 0) {
        return NULL;
    }

    int *types = malloc(sizeof(int) * parameter_count);
    if (types == NULL) {
        fprintf(stderr, "Erro interno: falha ao alocar tipos dos parametros\n");
        exit(1);
    }

    int index = 0;
    while (params != NULL && index < parameter_count) {
        ASTNode *item = params->list.item;

        if (item != NULL && item->type == AST_DECL) {
            types[index] = item->decl.data_type;
        } else {
            types[index] = TYPE_UNKNOWN;
        }

        index++;
        params = params->list.next;
    }

    return types;
}

static void attach_parameter_types(Symbol *function_symbol, ASTNode *params) {
    if (function_symbol == NULL || function_symbol->kind != SYMBOL_FUNCTION) {
        return;
    }

    function_symbol->parameter_types = copy_parameter_types(params, function_symbol->parameter_count);
}

static void validate_assignment_type(SemanticContext *ctx, const char *name,
                                     int expected_type, int actual_type) {
    if (!is_type_compatible(expected_type, actual_type)) {
        report_semantic_error(ctx,
                              "atribuicao incompativel para '%s' (esperado %s, encontrado %s)",
                              name,
                              type_to_string(expected_type),
                              type_to_string(actual_type));
    }
}

static void validate_condition_type(SemanticContext *ctx, ASTNode *condition, const char *command_name) {
    int condition_type = infer_expression_type(condition, ctx);

    if (condition_type != TYPE_UNKNOWN && !is_numeric_type(condition_type)) {
        report_semantic_error(ctx,
                              "condicao do %s deve ser numerica, encontrada %s",
                              command_name,
                              type_to_string(condition_type));
    }
}

static int is_lvalue(ASTNode *node) {
    return node != NULL && (node->type == AST_ID || node->type == AST_ARRAY_ACCESS);
}

static int infer_array_access_type(ASTNode *node, SemanticContext *ctx) {
    Symbol *symbol = require_array(ctx, node->array_access.var_name);
    int index_type = infer_expression_type(node->array_access.index, ctx);

    if (index_type != TYPE_UNKNOWN && index_type != INT) {
        report_semantic_error(ctx,
                              "indice do array '%s' deve ser int, encontrado %s",
                              node->array_access.var_name,
                              type_to_string(index_type));
    }

    return symbol != NULL ? symbol->data_type : TYPE_UNKNOWN;
}

static void validate_function_arguments(ASTNode *args, Symbol *function_symbol,
                                        const char *function_name, SemanticContext *ctx) {
    int expected_count = function_symbol != NULL ? function_symbol->parameter_count : 0;
    int actual_count = count_arguments(args);

    if (function_symbol == NULL || function_symbol->kind != SYMBOL_FUNCTION) {
        while (args != NULL) {
            infer_expression_type(args->list.item, ctx);
            args = args->list.next;
        }
        return;
    }

    if (expected_count != actual_count) {
        report_semantic_error(ctx,
                              "funcao '%s' esperava %d argumento(s), mas recebeu %d",
                              function_name,
                              expected_count,
                              actual_count);
    }

    int index = 0;
    while (args != NULL) {
        int actual_type = infer_expression_type(args->list.item, ctx);

        if (function_symbol->parameter_types != NULL && index < expected_count) {
            int expected_type = function_symbol->parameter_types[index];

            if (!is_type_compatible(expected_type, actual_type)) {
                report_semantic_error(ctx,
                                      "argumento %d da funcao '%s' incompativel (esperado %s, encontrado %s)",
                                      index + 1,
                                      function_name,
                                      type_to_string(expected_type),
                                      type_to_string(actual_type));
            }
        }

        index++;
        args = args->list.next;
    }
}

static int infer_function_call_type(ASTNode *node, SemanticContext *ctx) {
    Symbol *symbol = lookup_symbol(ctx->table, node->func_call.func_name);

    if (symbol == NULL) {
        report_semantic_error(ctx, "funcao '%s' nao declarado(a)", node->func_call.func_name);
        validate_function_arguments(node->func_call.args, NULL, node->func_call.func_name, ctx);
        return TYPE_UNKNOWN;
    }

    if (symbol->kind != SYMBOL_FUNCTION) {
        report_semantic_error(ctx, "'%s' nao e uma funcao", node->func_call.func_name);
        validate_function_arguments(node->func_call.args, NULL, node->func_call.func_name, ctx);
        return TYPE_UNKNOWN;
    }

    validate_function_arguments(node->func_call.args, symbol, node->func_call.func_name, ctx);
    return symbol->data_type;
}

/* Avalia uma expressao constante e retorna o valor inteiro (se possivel) */
static int evaluate_constant_expression(ASTNode *node, int *value, int *is_constant) {
    *is_constant = 1;
    
    switch (node->type) {
        case AST_NUM:
            *value = node->int_val;
            return 1;
            
        case AST_FLOAT:
            /* Para simplificar, nao avaliamos float aqui */
            *is_constant = 0;
            return 1;
            
        case AST_BINOP: {
            int left_val, right_val;
            int left_const, right_const;
            
            if (!evaluate_constant_expression(node->binop.left, &left_val, &left_const))
                return 0;
            if (!evaluate_constant_expression(node->binop.right, &right_val, &right_const))
                return 0;
            
            if (!left_const || !right_const) {
                *is_constant = 0;
                return 1;
            }
            
            switch (node->binop.operator) {
                case '+': *value = left_val + right_val; break;
                case '-': *value = left_val - right_val; break;
                case '*': *value = left_val * right_val; break;
                case '/': 
                    if (right_val == 0) {
                        *is_constant = 0;
                        return 0;
                    }
                    *value = left_val / right_val; 
                    break;
                default:
                    *is_constant = 0;
                    return 1;
            }
            return 1;
        }
        
        case AST_UNOP:
            if (node->unop.operator == '-') {
                int operand_val;
                int operand_const;
                if (!evaluate_constant_expression(node->unop.operand, &operand_val, &operand_const))
                    return 0;
                if (operand_const) {
                    *value = -operand_val;
                    return 1;
                }
            }
            *is_constant = 0;
            return 1;
            
        default:
            *is_constant = 0;
            return 1;
    }
}

static int infer_binary_operation_type(ASTNode *node, SemanticContext *ctx) {
    int left_type = infer_expression_type(node->binop.left, ctx);
    int right_type = infer_expression_type(node->binop.right, ctx);
    int operator = node->binop.operator;

    if (left_type == TYPE_UNKNOWN || right_type == TYPE_UNKNOWN) {
        return TYPE_UNKNOWN;
    }

    switch (operator) {
        case '+':
        case '-':
        case '*':
            if (!is_numeric_type(left_type) || !is_numeric_type(right_type)) {
                report_semantic_error(ctx,
                                      "operador '%s' requer operandos numericos",
                                      operator_to_string(operator));
                return TYPE_UNKNOWN;
            }
            return arithmetic_result_type(left_type, right_type);

        case '/':
            if (!is_numeric_type(left_type) || !is_numeric_type(right_type)) {
                report_semantic_error(ctx,
                                      "operador '/' requer operandos numericos",
                                      operator_to_string(operator));
                return TYPE_UNKNOWN;
            }
            
            /* Verificacao de divisao por zero constante */
            {
                int divisor_value;
                int is_constant;
                
                if (evaluate_constant_expression(node->binop.right, &divisor_value, &is_constant)) {
                    if (is_constant && divisor_value == 0) {
                        report_semantic_error(ctx, "divisao por zero constante");
                        return TYPE_UNKNOWN;
                    }
                }
            }
            
            return arithmetic_result_type(left_type, right_type);

        case '>':
        case '<':
        case GEQ:
        case LEQ:
        case EQ:
        case NEQ:
            if (!is_numeric_type(left_type) || !is_numeric_type(right_type)) {
                report_semantic_error(ctx,
                                      "operador '%s' requer operandos numericos",
                                      operator_to_string(operator));
                return TYPE_UNKNOWN;
            }
            return INT;

        case AND:
        case OR:
            if (!is_numeric_type(left_type) || !is_numeric_type(right_type)) {
                report_semantic_error(ctx,
                                      "operador '%s' requer operandos numericos",
                                      operator_to_string(operator));
                return TYPE_UNKNOWN;
            }
            return INT;

        default:
            return TYPE_UNKNOWN;
    }
}

static int infer_unary_operation_type(ASTNode *node, SemanticContext *ctx) {
    int operand_type = infer_expression_type(node->unop.operand, ctx);
    int operator = node->unop.operator;

    if (operand_type == TYPE_UNKNOWN) {
        return TYPE_UNKNOWN;
    }

    switch (operator) {
        case '-':
            if (!is_numeric_type(operand_type)) {
                report_semantic_error(ctx, "operador '-' requer operando numerico");
                return TYPE_UNKNOWN;
            }

            return operand_type == CHAR ? INT : operand_type;

        case NOT:
            if (!is_numeric_type(operand_type)) {
                report_semantic_error(ctx, "operador '!' requer operando numerico");
                return TYPE_UNKNOWN;
            }

            return INT;

        default:
            return TYPE_UNKNOWN;
    }
}

static int infer_inc_dec_type(ASTNode *node, SemanticContext *ctx) {
    int operand_type = infer_expression_type(node->inc_dec.operand, ctx);
    const char* op_name = (node->type == AST_PRE_INC || node->type == AST_POST_INC) ? "++" : "--";
    
    if (!is_lvalue(node->inc_dec.operand)) {
        report_semantic_error(ctx,
                              "operador '%s' requer variavel ou acesso a array",
                              op_name);
        return TYPE_UNKNOWN;
    }
    
    if (!is_numeric_type(operand_type)) {
        report_semantic_error(ctx,
                              "operador '%s' requer operando numerico",
                              op_name);
        return TYPE_UNKNOWN;
    }
    
    return operand_type;
}

static int infer_expression_type(ASTNode *node, SemanticContext *ctx) {
    if (node == NULL) {
        return TYPE_UNKNOWN;
    }

    switch (node->type) {
        case AST_NUM:
            return INT;

        case AST_FLOAT:
            return FLOAT;

        case AST_CHAR_LITERAL:
            return CHAR;

        case AST_ID: {
            Symbol *symbol = require_variable(ctx, node->var_name);
            return symbol != NULL ? symbol->data_type : TYPE_UNKNOWN;
        }

        case AST_BINOP:
            return infer_binary_operation_type(node, ctx);

        case AST_UNOP:
            return infer_unary_operation_type(node, ctx);

        case AST_PRE_INC:
        case AST_PRE_DEC:
        case AST_POST_INC:
        case AST_POST_DEC:
            return infer_inc_dec_type(node, ctx);

        case AST_FUNC_CALL:
            return infer_function_call_type(node, ctx);

        case AST_ARRAY_ACCESS:
            return infer_array_access_type(node, ctx);

        default:
            analyze_node(node, ctx);
            return TYPE_UNKNOWN;
    }
}

static void analyze_list_as_parameters(ASTNode *list, SemanticContext *ctx) {
    while (list != NULL) {
        ASTNode *item = list->list.item;

        if (item != NULL && item->type == AST_DECL) {
            if (is_void_type(item->decl.data_type)) {
                report_semantic_error(ctx,
                                      "parametro '%s' nao pode ter tipo void",
                                      item->decl.var_name);
            }

            declare_symbol(ctx, item->decl.var_name, item->decl.data_type,
                           SYMBOL_PARAMETER, 0, 0);
        }

        list = list->list.next;
    }
}

static void validate_array_initializers(ASTNode *values, SemanticContext *ctx,
                                        const char *array_name, int array_type, int array_size) {
    int count = 0;

    while (values != NULL) {
        count++;
        int value_type = infer_expression_type(values->list.item, ctx);

        if (!is_type_compatible(array_type, value_type)) {
            report_semantic_error(ctx,
                                  "inicializacao incompativel no array '%s' na posicao %d (esperado %s, encontrado %s)",
                                  array_name,
                                  count,
                                  type_to_string(array_type),
                                  type_to_string(value_type));
        }

        values = values->list.next;
    }

    if (array_size >= 0 && count > array_size) {
        report_semantic_error(ctx,
                              "array '%s' possui tamanho %d, mas recebeu %d valor(es)",
                              array_name,
                              array_size,
                              count);
    }
}

static int body_guarantees_return(ASTNode *node) {
    if (node == NULL) {
        return 0;
    }

    switch (node->type) {
        case AST_RETURN:
            return 1;

        case AST_SEQ:
            return body_guarantees_return(node->seq.first) ||
                   body_guarantees_return(node->seq.next);

        case AST_BLOCK:
            return body_guarantees_return(node->block.statements);

        case AST_IF:
            return node->if_stmt.else_body != NULL &&
                   body_guarantees_return(node->if_stmt.if_body) &&
                   body_guarantees_return(node->if_stmt.else_body);

        default:
            return 0;
    }
}

static void analyze_assignment(ASTNode *node, SemanticContext *ctx) {
    Symbol *symbol = require_variable(ctx, node->assign.var_name);
    int value_type = infer_expression_type(node->assign.value, ctx);

    if (symbol != NULL) {
        validate_assignment_type(ctx, node->assign.var_name, symbol->data_type, value_type);
    }
}

static void analyze_array_assignment(ASTNode *node, SemanticContext *ctx) {
    Symbol *symbol = require_array(ctx, node->assign_array.var_name);
    int index_type = infer_expression_type(node->assign_array.index, ctx);
    int value_type = infer_expression_type(node->assign_array.value, ctx);

    if (index_type != TYPE_UNKNOWN && index_type != INT) {
        report_semantic_error(ctx,
                              "indice do array '%s' deve ser int, encontrado %s",
                              node->assign_array.var_name,
                              type_to_string(index_type));
    }

    if (symbol != NULL) {
        validate_assignment_type(ctx, node->assign_array.var_name, symbol->data_type, value_type);
    }
}

static void analyze_declaration(ASTNode *node, SemanticContext *ctx) {
    if (node == NULL) return;

    /* Para listas de declaracoes (declaracoes multiplas) */
    if (node->type == AST_LIST) {
        analyze_node(node, ctx);
        return;
    }

    if (is_void_type(node->decl.data_type)) {
        report_semantic_error(ctx,
                              "variavel '%s' nao pode ter tipo void",
                              node->decl.var_name);
    }

    Symbol *symbol = declare_symbol(ctx, node->decl.var_name, node->decl.data_type,
                                    SYMBOL_VARIABLE, 0, 0);
    int value_type = infer_expression_type(node->decl.value, ctx);

    if (symbol != NULL && node->decl.value != NULL) {
        validate_assignment_type(ctx, node->decl.var_name, node->decl.data_type, value_type);
    }
}

/* Funcao para calcular tamanho de array a partir dos inicializadores */
static int calculate_array_size_from_initializers(ASTNode *values) {
    int count = 0;
    while (values != NULL) {
        count++;
        values = values->list.next;
    }
    return count;
}

static void analyze_array_declaration(ASTNode *node, SemanticContext *ctx) {
    if (is_void_type(node->array_decl.data_type)) {
        report_semantic_error(ctx,
                              "array '%s' nao pode ter tipo void",
                              node->array_decl.var_name);
    }

    int array_size = node->array_decl.size;
    
    /* Se tamanho foi omitido (size = -1), calcular a partir dos inicializadores */
    if (array_size == -1) {
        array_size = calculate_array_size_from_initializers(node->array_decl.values);
        if (array_size == 0) {
            report_semantic_error(ctx,
                                  "array '%s' com tamanho omitido deve ter inicializadores",
                                  node->array_decl.var_name);
            return;
        }
    }
    
    if (array_size <= 0) {
        report_semantic_error(ctx,
                              "array '%s' deve ter tamanho maior que zero",
                              node->array_decl.var_name);
    }

    declare_symbol(ctx, node->array_decl.var_name, node->array_decl.data_type,
                   SYMBOL_ARRAY, array_size, 0);
    validate_array_initializers(node->array_decl.values, ctx,
                                node->array_decl.var_name,
                                node->array_decl.data_type,
                                array_size);
}

static void analyze_function_prototype(ASTNode *node, SemanticContext *ctx) {
    if (node == NULL) return;
    
    int parameter_count = count_list(node->func_decl.params);
    
    /* Fora de qualquer escopo (escopo global) */
    int old_scope = ctx->table->current_scope;
    ctx->table->current_scope = 0;
    
    declare_symbol(ctx, node->func_decl.func_name, node->func_decl.return_type,
                   SYMBOL_FUNCTION, 0, parameter_count);
    
    ctx->table->current_scope = old_scope;
}

static void analyze_function_declaration(ASTNode *node, SemanticContext *ctx) {
    int parameter_count = count_list(node->func_decl.params);
    
    /* Fora de qualquer escopo (escopo global) */
    int old_scope = ctx->table->current_scope;
    ctx->table->current_scope = 0;
    
    Symbol *function_symbol = declare_symbol(ctx, node->func_decl.func_name,
                                             node->func_decl.return_type,
                                             SYMBOL_FUNCTION, 0, parameter_count);
    
    ctx->table->current_scope = old_scope;
    
    if (function_symbol != NULL) {
        attach_parameter_types(function_symbol, node->func_decl.params);
    }

    int previous_inside_function = ctx->inside_function;
    int previous_return_type = ctx->current_function_return_type;
    const char *previous_function_name = ctx->current_function_name;

    ctx->inside_function = 1;
    ctx->current_function_return_type = node->func_decl.return_type;
    ctx->current_function_name = node->func_decl.func_name;

    enter_scope(ctx->table);
    analyze_list_as_parameters(node->func_decl.params, ctx);
    analyze_node(node->func_decl.body, ctx);
    exit_scope(ctx->table);

    if (!is_void_type(node->func_decl.return_type) && !body_guarantees_return(node->func_decl.body)) {
        report_semantic_error(ctx,
                              "funcao '%s' deve retornar um valor do tipo %s",
                              node->func_decl.func_name,
                              type_to_string(node->func_decl.return_type));
    }

    ctx->inside_function = previous_inside_function;
    ctx->current_function_return_type = previous_return_type;
    ctx->current_function_name = previous_function_name;
}

static void analyze_return(ASTNode *node, SemanticContext *ctx) {
    if (!ctx->inside_function) {
        report_semantic_error(ctx, "return fora de funcao");
        infer_expression_type(node->return_stmt.expr, ctx);
        return;
    }

    if (node->return_stmt.expr == NULL) {
        if (!is_void_type(ctx->current_function_return_type)) {
            report_semantic_error(ctx,
                                  "return sem valor na funcao '%s' (esperado %s)",
                                  ctx->current_function_name,
                                  type_to_string(ctx->current_function_return_type));
        }
        return;
    }

    int return_type = infer_expression_type(node->return_stmt.expr, ctx);

    if (is_void_type(ctx->current_function_return_type)) {
        report_semantic_error(ctx,
                              "funcao void '%s' nao deve retornar valor",
                              ctx->current_function_name);
    } else if (!is_type_compatible(ctx->current_function_return_type, return_type)) {
        report_semantic_error(ctx,
                              "retorno incompativel na funcao '%s' (esperado %s, encontrado %s)",
                              ctx->current_function_name,
                              type_to_string(ctx->current_function_return_type),
                              type_to_string(return_type));
    }
}

static void analyze_node(ASTNode *node, SemanticContext *ctx) {
    if (node == NULL) return;

    switch (node->type) {
        case AST_NUM:
        case AST_FLOAT:
        case AST_CHAR_LITERAL:
        case AST_ID:
        case AST_BINOP:
        case AST_UNOP:
        case AST_FUNC_CALL:
        case AST_ARRAY_ACCESS:
        case AST_PRE_INC:
        case AST_PRE_DEC:
        case AST_POST_INC:
        case AST_POST_DEC:
            infer_expression_type(node, ctx);
            break;

        case AST_ASSIGN:
            analyze_assignment(node, ctx);
            break;

        case AST_ASSIGN_ARRAY:
            analyze_array_assignment(node, ctx);
            break;

        case AST_IF:
            validate_condition_type(ctx, node->if_stmt.condition, "if");
            analyze_node(node->if_stmt.if_body, ctx);
            analyze_node(node->if_stmt.else_body, ctx);
            break;

        case AST_WHILE:
            validate_condition_type(ctx, node->while_stmt.condition, "while");
            analyze_node(node->while_stmt.body, ctx);
            break;

        case AST_FOR:
            /* Cria um novo escopo para o for se houver declaracao no init */
            if (node->for_stmt.init != NULL && 
                (node->for_stmt.init->type == AST_BLOCK)) {
                enter_scope(ctx->table);
            }
            
            /* Analisa o init (pode ser bloco com declaracoes) */
            if (node->for_stmt.init != NULL)
                analyze_node(node->for_stmt.init, ctx);
            
            /* Analisa a condicao (pode ser NULL = sempre verdadeiro) */
            if (node->for_stmt.condition != NULL) {
                validate_condition_type(ctx, node->for_stmt.condition, "for");
            }
            
            /* Analisa o update */
            if (node->for_stmt.update != NULL)
                analyze_node(node->for_stmt.update, ctx);
            
            /* Analisa o corpo */
            analyze_node(node->for_stmt.body, ctx);
            
            /* Sai do escopo se foi criado */
            if (node->for_stmt.init != NULL && 
                (node->for_stmt.init->type == AST_BLOCK)) {
                exit_scope(ctx->table);
            }
            break;

        case AST_SEQ:
            analyze_node(node->seq.first, ctx);
            analyze_node(node->seq.next, ctx);
            break;

        case AST_BLOCK:
            enter_scope(ctx->table);
            analyze_node(node->block.statements, ctx);
            exit_scope(ctx->table);
            break;

        case AST_DECL:
            analyze_declaration(node, ctx);
            break;

        case AST_ARRAY_DECL:
            analyze_array_declaration(node, ctx);
            break;

        case AST_FUNC_DECL:
            if (node->func_decl.body == NULL) {
                analyze_function_prototype(node, ctx);
            } else {
                analyze_function_declaration(node, ctx);
            }
            break;

        case AST_RETURN:
            analyze_return(node, ctx);
            break;

        case AST_LIST:
            analyze_node(node->list.item, ctx);
            analyze_node(node->list.next, ctx);
            break;

        default:
            break;
    }
}

void analyze_semantics(ASTNode *root, SymbolTable *table) {
    SemanticContext ctx;

    ctx.table = table;
    ctx.inside_function = 0;
    ctx.current_function_return_type = TYPE_UNKNOWN;
    ctx.current_function_name = NULL;
    ctx.semantic_errors = 0;

    /* Analisar diretamente (sem dois passes) */
    analyze_node(root, &ctx);
    
    table->semantic_errors = ctx.semantic_errors;
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

void free_symbol_table(SymbolTable *table) {
    Symbol *symbol = table->head;

    while (symbol != NULL) {
        Symbol *next = symbol->next;
        free(symbol->name);
        free(symbol->parameter_types);
        free(symbol);
        symbol = next;
    }

    table->head = NULL;
    table->current_scope = 0;
    table->next_scope = 1;
    table->scope_stack[0] = 0;
    table->scope_stack_top = 0;
    table->semantic_errors = 0;
}