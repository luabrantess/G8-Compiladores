// Teste 9: Laco for e expressoes complexas
#include <stdio.h>

int main() {
    int x;
    for (x = 0; x <= 100; x = x + 1) {
        if (x >= 10 && x < 50) {
            x = x * 2;
        }
    }

    return 0;
}

// Saida esperada:
// Iniciando analise sintatica...
// Analise concluida com sucesso!
// Analise semantica concluida sem erros.
// Codigo Lua gerado em: saida.lua

// Saida recebida:
// Iniciando analise sintatica...
// Analise concluida com sucesso!
// Analise semantica concluida sem erros.
// Codigo Lua gerado em: saida.lua
// Resultado: PASSOU.
