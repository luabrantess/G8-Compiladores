// Teste 10: Erro lexico e if sem parenteses
#include <stdio.h>

int main() {
    int z = 10;
    z = z @ 2;

    if z > 5 {
        z = 0;
    }

    return 0;
}

// Saida esperada:
// Iniciando analise sintatica...
// Erro lexico na linha 6: @
// Erro sintatico na linha 6: syntax error
// A analise falhou devido a erros lexicos.

// Saida recebida:
// Iniciando analise sintatica...
// Erro lexico na linha 6: @
// Erro sintatico na linha 6: syntax error
// A analise falhou devido a erros lexicos.
// Resultado: PASSOU como teste invalido.
// Falhou no lexer por causa do caractere @ e depois no parser pela expressao quebrada.
