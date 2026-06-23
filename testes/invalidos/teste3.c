// Teste 3: Erros sintaticos diversos
#include <stdio.h>

int main() {
    while x > 0 { }
    int = 10;
    float y = ;
    if (x > )

    return 0;
}

// Saida esperada:
// Iniciando analise sintatica...
// Erro sintatico: syntax error
// A analise falhou devido a erros sintaticos.

// Saida recebida:
// Iniciando analise sintatica...
// Erro sintatico na linha 5: syntax error
// A analise falhou devido a erros sintaticos.
// Resultado: PASSOU como teste invalido.
// Falhou no parser porque o while esta sem parenteses: while x > 0 { }.
