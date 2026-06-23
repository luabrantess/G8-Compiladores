// Teste 4: Lista e for-in
#include <stdio.h>

int main() {
    int x[] = {1, 2, 3, 4, 5};

    for i in x {
        printf("%d", i);
    }

    return 0;
}

// Saida esperada:
// Iniciando analise sintatica...
// Erro sintatico: syntax error
// A analise falhou devido a erros sintaticos.

// Saida recebida:
// Iniciando analise sintatica...
// Erro sintatico na linha 7: syntax error
// A analise falhou devido a erros sintaticos.
// Resultado: PASSOU como teste invalido.
// Falhou porque a gramatica nao reconhece for-in: for i in x { ... }.
