#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen_lua.h"
#include "parser.tab.h"

typedef struct {
    FILE *out;
    int indent;
    int inside_function;
    int generated_main;
} LuaCodegenContext;

static void emit_indent(LuaCodegenContext *ctx) {
    for (int i = 0; i < ctx->indent; i++) {
        fputs("    ", ctx->out);
    }
}

static void emit_newline(LuaCodegenContext *ctx) {
    fputc('\n', ctx->out);
}

static int is_list_node(ASTNode *node) {
    return node != NULL && node->type == AST_LIST;
}

static int is_statement_expression(ASTNode *node) {
    if (node == NULL) return 0;

    return node->type == AST_FUNC_CALL ||
           node->type == AST_PRE_INC ||
           node->type == AST_PRE_DEC ||
           node->type == AST_POST_INC ||
           node->type == AST_POST_DEC;
}

static void generate_statement(ASTNode *node, LuaCodegenContext *ctx);
static void generate_statement_list(ASTNode *node, LuaCodegenContext *ctx);
static void generate_expression(ASTNode *node, LuaCodegenContext *ctx);
static void generate_lvalue(ASTNode *node, LuaCodegenContext *ctx);

static const char *binary_operator_to_lua(int operator) {
    switch (operator) {
        case '+': return "+";
        case '-': return "-";
        case '*': return "*";
        case '/': return "/";
        case '%': return "%";
        case '>': return ">";
        case '<': return "<";
        case EQ: return "==";
        case NEQ: return "~=";
        case GEQ: return ">=";
        case LEQ: return "<=";
        case AND: return "and";
        case OR: return "or";
        default: return "?";
    }
}

static void emit_lua_string_literal(FILE *out, const char *text) {
    fputc('"', out);

    if (text != NULL) {
        for (const unsigned char *p = (const unsigned char *)text; *p != '\0'; p++) {
            switch (*p) {
                case '\n': fputs("\\n", out); break;
                case '\t': fputs("\\t", out); break;
                case '\r': fputs("\\r", out); break;
                case '\\': fputs("\\\\", out); break;
                case '"':  fputs("\\\"", out); break;
                default:
                    if (*p < 32 || *p > 126) {
                        fprintf(out, "\\%03u", *p);
                    } else {
                        fputc(*p, out);
                    }
                    break;
            }
        }
    }

    fputc('"', out);
}

static void generate_default_value(int data_type, LuaCodegenContext *ctx) {
    switch (data_type) {
        case CHAR:
        case INT:
        case FLOAT:
        case DOUBLE:
            fputs("0", ctx->out);
            break;
        default:
            fputs("nil", ctx->out);
            break;
    }
}

static void generate_params(ASTNode *params, LuaCodegenContext *ctx) {
    int first = 1;

    while (params != NULL) {
        ASTNode *item = params->list.item;

        if (item != NULL && item->type == AST_DECL) {
            if (!first) {
                fputs(", ", ctx->out);
            }
            fputs(item->decl.var_name, ctx->out);
            first = 0;
        }

        params = params->list.next;
    }
}

static void generate_argument_expressions(ASTNode *args, LuaCodegenContext *ctx) {
    int first = 1;

    while (args != NULL) {
        if (!first) {
            fputs(", ", ctx->out);
        }

        generate_expression(args->list.item, ctx);
        first = 0;
        args = args->list.next;
    }
}

static void generate_printf_format_call(ASTNode *args, LuaCodegenContext *ctx) {
    fputs("string.format(", ctx->out);

    if (args == NULL) {
        emit_lua_string_literal(ctx->out, "");
    } else {
        generate_expression(args->list.item, ctx);

        ASTNode *extra = args->list.next;
        while (extra != NULL) {
            fputs(", ", ctx->out);
            generate_expression(extra->list.item, ctx);
            extra = extra->list.next;
        }
    }

    fputc(')', ctx->out);
}

static void generate_printf_statement_call(ASTNode *args, LuaCodegenContext *ctx) {
    fputs("io.write(", ctx->out);
    generate_printf_format_call(args, ctx);
    fputc(')', ctx->out);
}

static void generate_printf_expression_call(ASTNode *args, LuaCodegenContext *ctx) {
    fputs("(function() local __printf_text = ", ctx->out);
    generate_printf_format_call(args, ctx);
    fputs("; io.write(__printf_text); return #__printf_text end)()", ctx->out);
}

static void generate_lvalue(ASTNode *node, LuaCodegenContext *ctx) {
    if (node == NULL) {
        fputs("nil", ctx->out);
        return;
    }

    switch (node->type) {
        case AST_ID:
            fputs(node->var_name, ctx->out);
            break;

        case AST_ARRAY_ACCESS:
            fputs(node->array_access.var_name, ctx->out);
            fputc('[', ctx->out);
            generate_expression(node->array_access.index, ctx);
            fputc(']', ctx->out);
            break;

        default:
            generate_expression(node, ctx);
            break;
    }
}

static void generate_inc_dec_expression(ASTNode *node, LuaCodegenContext *ctx) {
    const char *op = (node->type == AST_PRE_INC || node->type == AST_POST_INC) ? "+" : "-";
    int is_pre = node->type == AST_PRE_INC || node->type == AST_PRE_DEC;

    if (is_pre) {
        fputs("(function() ", ctx->out);
        generate_lvalue(node->inc_dec.operand, ctx);
        fprintf(ctx->out, " = ");
        generate_lvalue(node->inc_dec.operand, ctx);
        fprintf(ctx->out, " %s 1; return ", op);
        generate_lvalue(node->inc_dec.operand, ctx);
        fputs(" end)()", ctx->out);
    } else {
        fputs("(function() local __old = ", ctx->out);
        generate_lvalue(node->inc_dec.operand, ctx);
        fputs("; ", ctx->out);
        generate_lvalue(node->inc_dec.operand, ctx);
        fprintf(ctx->out, " = ");
        generate_lvalue(node->inc_dec.operand, ctx);
        fprintf(ctx->out, " %s 1; return __old end)()", op);
    }
}

static void generate_expression(ASTNode *node, LuaCodegenContext *ctx) {
    if (node == NULL) {
        fputs("nil", ctx->out);
        return;
    }

    switch (node->type) {
        case AST_NUM:
            fprintf(ctx->out, "%d", node->int_val);
            break;

        case AST_FLOAT:
            fprintf(ctx->out, "%g", node->float_val);
            break;

        case AST_CHAR_LITERAL:
            fprintf(ctx->out, "%d", (unsigned char)node->char_val);
            break;

        case AST_STRING_LITERAL:
            emit_lua_string_literal(ctx->out, node->string_val);
            break;

        case AST_ID:
            fputs(node->var_name, ctx->out);
            break;

        case AST_ARRAY_ACCESS:
            generate_lvalue(node, ctx);
            break;

        case AST_BINOP:
            fputc('(', ctx->out);
            generate_expression(node->binop.left, ctx);
            fprintf(ctx->out, " %s ", binary_operator_to_lua(node->binop.operator));
            generate_expression(node->binop.right, ctx);
            fputc(')', ctx->out);
            break;

        case AST_UNOP:
            if (node->unop.operator == NOT) {
                fputs("(not ", ctx->out);
                generate_expression(node->unop.operand, ctx);
                fputc(')', ctx->out);
            } else {
                fputc('(', ctx->out);
                fputc(node->unop.operator, ctx->out);
                generate_expression(node->unop.operand, ctx);
                fputc(')', ctx->out);
            }
            break;

        case AST_PRE_INC:
        case AST_PRE_DEC:
        case AST_POST_INC:
        case AST_POST_DEC:
            generate_inc_dec_expression(node, ctx);
            break;

        case AST_FUNC_CALL:
            if (strcmp(node->func_call.func_name, "printf") == 0) {
                generate_printf_expression_call(node->func_call.args, ctx);
            } else {
                fputs(node->func_call.func_name, ctx->out);
                fputc('(', ctx->out);
                generate_argument_expressions(node->func_call.args, ctx);
                fputc(')', ctx->out);
            }
            break;

        default:
            fputs("nil", ctx->out);
            break;
    }
}

static void generate_inc_dec_statement(ASTNode *node, LuaCodegenContext *ctx) {
    const char *op = (node->type == AST_PRE_INC || node->type == AST_POST_INC) ? "+" : "-";

    emit_indent(ctx);
    generate_lvalue(node->inc_dec.operand, ctx);
    fprintf(ctx->out, " = ");
    generate_lvalue(node->inc_dec.operand, ctx);
    fprintf(ctx->out, " %s 1", op);
    emit_newline(ctx);
}

static void generate_declaration(ASTNode *node, LuaCodegenContext *ctx) {
    emit_indent(ctx);

    if (ctx->inside_function) {
        fputs("local ", ctx->out);
    }

    fputs(node->decl.var_name, ctx->out);
    fputs(" = ", ctx->out);

    if (node->decl.value != NULL) {
        generate_expression(node->decl.value, ctx);
    } else {
        generate_default_value(node->decl.data_type, ctx);
    }

    emit_newline(ctx);
}

static void generate_array_values(ASTNode *values, LuaCodegenContext *ctx) {
    int index = 0;
    int first = 1;

    fputc('{', ctx->out);

    while (values != NULL) {
        if (!first) {
            fputs(", ", ctx->out);
        }

        fprintf(ctx->out, "[%d] = ", index);
        generate_expression(values->list.item, ctx);

        first = 0;
        index++;
        values = values->list.next;
    }

    fputc('}', ctx->out);
}

static void generate_array_declaration(ASTNode *node, LuaCodegenContext *ctx) {
    emit_indent(ctx);

    if (ctx->inside_function) {
        fputs("local ", ctx->out);
    }

    fputs(node->array_decl.var_name, ctx->out);
    fputs(" = ", ctx->out);

    if (node->array_decl.values != NULL) {
        generate_array_values(node->array_decl.values, ctx);
    } else {
        fputs("{}", ctx->out);
    }

    emit_newline(ctx);
}

static void generate_block_body(ASTNode *body, LuaCodegenContext *ctx) {
    ctx->indent++;
    generate_statement_list(body, ctx);
    ctx->indent--;
}

static void generate_if_statement(ASTNode *node, LuaCodegenContext *ctx) {
    emit_indent(ctx);
    fputs("if ", ctx->out);
    generate_expression(node->if_stmt.condition, ctx);
    fputs(" then", ctx->out);
    emit_newline(ctx);

    generate_block_body(node->if_stmt.if_body, ctx);

    if (node->if_stmt.else_body != NULL) {
        emit_indent(ctx);
        fputs("else", ctx->out);
        emit_newline(ctx);
        generate_block_body(node->if_stmt.else_body, ctx);
    }

    emit_indent(ctx);
    fputs("end", ctx->out);
    emit_newline(ctx);
}

static void generate_while_statement(ASTNode *node, LuaCodegenContext *ctx) {
    emit_indent(ctx);
    fputs("while ", ctx->out);
    generate_expression(node->while_stmt.condition, ctx);
    fputs(" do", ctx->out);
    emit_newline(ctx);

    generate_block_body(node->while_stmt.body, ctx);

    emit_indent(ctx);
    fputs("end", ctx->out);
    emit_newline(ctx);
}

static void generate_for_statement(ASTNode *node, LuaCodegenContext *ctx) {
    emit_indent(ctx);
    fputs("do", ctx->out);
    emit_newline(ctx);

    ctx->indent++;

    if (node->for_stmt.init != NULL) {
        generate_statement(node->for_stmt.init, ctx);
    }

    emit_indent(ctx);
    fputs("while ", ctx->out);
    if (node->for_stmt.condition != NULL) {
        generate_expression(node->for_stmt.condition, ctx);
    } else {
        fputs("true", ctx->out);
    }
    fputs(" do", ctx->out);
    emit_newline(ctx);

    ctx->indent++;
    generate_statement_list(node->for_stmt.body, ctx);
    if (node->for_stmt.update != NULL) {
        generate_statement(node->for_stmt.update, ctx);
    }
    ctx->indent--;

    emit_indent(ctx);
    fputs("end", ctx->out);
    emit_newline(ctx);

    ctx->indent--;

    emit_indent(ctx);
    fputs("end", ctx->out);
    emit_newline(ctx);
}

static void generate_function(ASTNode *node, LuaCodegenContext *ctx) {
    if (node->func_decl.body == NULL) {
        return;
    }

    if (strcmp(node->func_decl.func_name, "main") == 0) {
        ctx->generated_main = 1;
    }

    emit_indent(ctx);
    fprintf(ctx->out, "function %s(", node->func_decl.func_name);
    generate_params(node->func_decl.params, ctx);
    fputs(")", ctx->out);
    emit_newline(ctx);

    int previous_inside_function = ctx->inside_function;
    ctx->inside_function = 1;
    generate_block_body(node->func_decl.body, ctx);
    ctx->inside_function = previous_inside_function;

    emit_indent(ctx);
    fputs("end", ctx->out);
    emit_newline(ctx);
    emit_newline(ctx);
}

static void generate_assignment(ASTNode *node, LuaCodegenContext *ctx) {
    emit_indent(ctx);
    fputs(node->assign.var_name, ctx->out);
    fputs(" = ", ctx->out);
    generate_expression(node->assign.value, ctx);
    emit_newline(ctx);
}

static void generate_array_assignment(ASTNode *node, LuaCodegenContext *ctx) {
    emit_indent(ctx);
    fputs(node->assign_array.var_name, ctx->out);
    fputc('[', ctx->out);
    generate_expression(node->assign_array.index, ctx);
    fputs("] = ", ctx->out);
    generate_expression(node->assign_array.value, ctx);
    emit_newline(ctx);
}

static void generate_return(ASTNode *node, LuaCodegenContext *ctx) {
    emit_indent(ctx);
    fputs("return", ctx->out);

    if (node->return_stmt.expr != NULL) {
        fputc(' ', ctx->out);
        generate_expression(node->return_stmt.expr, ctx);
    }

    emit_newline(ctx);
}

static void generate_statement(ASTNode *node, LuaCodegenContext *ctx) {
    if (node == NULL) {
        return;
    }

    switch (node->type) {
        case AST_SEQ:
            generate_statement_list(node, ctx);
            break;

        case AST_LIST:
            generate_statement_list(node, ctx);
            break;

        case AST_BLOCK:
            emit_indent(ctx);
            fputs("do", ctx->out);
            emit_newline(ctx);
            generate_block_body(node->block.statements, ctx);
            emit_indent(ctx);
            fputs("end", ctx->out);
            emit_newline(ctx);
            break;

        case AST_DECL:
            generate_declaration(node, ctx);
            break;

        case AST_ARRAY_DECL:
            generate_array_declaration(node, ctx);
            break;

        case AST_FUNC_DECL:
            generate_function(node, ctx);
            break;

        case AST_ASSIGN:
            generate_assignment(node, ctx);
            break;

        case AST_ASSIGN_ARRAY:
            generate_array_assignment(node, ctx);
            break;

        case AST_IF:
            generate_if_statement(node, ctx);
            break;

        case AST_WHILE:
            generate_while_statement(node, ctx);
            break;

        case AST_FOR:
            generate_for_statement(node, ctx);
            break;

        case AST_RETURN:
            generate_return(node, ctx);
            break;

        case AST_FUNC_CALL:
            emit_indent(ctx);
            if (strcmp(node->func_call.func_name, "printf") == 0) {
                generate_printf_statement_call(node->func_call.args, ctx);
            } else {
                generate_expression(node, ctx);
            }
            emit_newline(ctx);
            break;

        case AST_PRE_INC:
        case AST_PRE_DEC:
        case AST_POST_INC:
        case AST_POST_DEC:
            generate_inc_dec_statement(node, ctx);
            break;

        default:
            if (is_statement_expression(node)) {
                emit_indent(ctx);
                generate_expression(node, ctx);
                emit_newline(ctx);
            } else {
                emit_indent(ctx);
                fputs("-- expressao sem efeito omitida", ctx->out);
                emit_newline(ctx);
            }
            break;
    }
}

static void generate_statement_list(ASTNode *node, LuaCodegenContext *ctx) {
    if (node == NULL) {
        return;
    }

    if (node->type == AST_SEQ) {
        generate_statement_list(node->seq.first, ctx);
        generate_statement(node->seq.next, ctx);
        return;
    }

    if (node->type == AST_LIST) {
        ASTNode *current = node;
        while (current != NULL) {
            generate_statement(current->list.item, ctx);
            current = current->list.next;
        }
        return;
    }

    generate_statement(node, ctx);
}

void generate_lua(ASTNode *root, const char *output_file) {
    FILE *out = fopen(output_file, "w");
    if (out == NULL) {
        fprintf(stderr, "Erro ao criar arquivo Lua '%s'\n", output_file);
        return;
    }

    LuaCodegenContext ctx;
    ctx.out = out;
    ctx.indent = 0;
    ctx.inside_function = 0;
    ctx.generated_main = 0;

    fputs("-- Codigo Lua gerado automaticamente pelo compilador C para Lua\n\n", out);

    generate_statement_list(root, &ctx);

    if (ctx.generated_main) {
        fputs("main()\n", out);
    }

    fclose(out);
}
