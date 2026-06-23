// Teste 7: Erro por falta de ponto e virgula
#include <stdio.h>

int main() {
    int y = 20
    y = y + 1;

    return 0;
}

// Saida esperada:
// Iniciando analise sintatica...
// Erro sintatico: syntax error
// A analise falhou devido a erros sintaticos.

// Saida recebida:
// Iniciando analise sintatica...
// Erro sintatico na linha 6: syntax error
// A analise falhou devido a erros sintaticos.
// Resultado: PASSOU como teste invalido.
// Falhou porque falta ponto e virgula apos: int y = 20.
