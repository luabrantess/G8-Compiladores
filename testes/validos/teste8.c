// Teste 8: Condicionais e repeticao com while
#include <stdio.h>

int main() {
    int i = 0;
    while (i < 10) {
        if (i == 5) {
            i = i + 2;
        } else {
            i = i + 1;
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
