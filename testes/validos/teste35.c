// Teste 35: Geracao Lua para array, acesso e atribuicao por indice
// Entrada em C:
#include <stdio.h>

int main() {
    int valores[] = {1, 2, 3};
    valores[1] = valores[0] + valores[2];
    return valores[1];
}

// Codigo Lua esperado:
// -- Codigo Lua gerado automaticamente pelo compilador C para Lua
//
// function main()
//     local valores = {[0] = 1, [1] = 2, [2] = 3}
//     valores[1] = (valores[0] + valores[2])
//     return valores[1]
// end
//
// main()
