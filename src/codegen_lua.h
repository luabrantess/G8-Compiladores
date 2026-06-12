#ifndef CODEGEN_LUA_H
#define CODEGEN_LUA_H

#include "ast.h"

void generate_lua(ASTNode *root, const char *output_file);

#endif
