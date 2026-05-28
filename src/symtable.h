#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "ast.h"

typedef enum {
    SYMBOL_VARIABLE,
    SYMBOL_ARRAY,
    SYMBOL_FUNCTION,
    SYMBOL_PARAMETER
} SymbolKind;

typedef struct Symbol {
    char *name;
    int data_type;
    SymbolKind kind;
    int scope;
    int array_size;
    int parameter_count;
    int *parameter_types;
    struct Symbol *next;
} Symbol;

typedef struct {
    Symbol *head;
    int current_scope;
    int next_scope;
    int scope_stack[256];
    int scope_stack_top;
    int semantic_errors;
} SymbolTable;

void init_symbol_table(SymbolTable *table);
void enter_scope(SymbolTable *table);
void exit_scope(SymbolTable *table);

Symbol *lookup_symbol(SymbolTable *table, const char *name);
Symbol *lookup_symbol_in_current_scope(SymbolTable *table, const char *name);

int insert_symbol(SymbolTable *table, const char *name, int data_type,
                  SymbolKind kind, int array_size, int parameter_count);

void analyze_semantics(ASTNode *root, SymbolTable *table);
void print_symbol_table(SymbolTable *table);
void free_symbol_table(SymbolTable *table);

#endif
