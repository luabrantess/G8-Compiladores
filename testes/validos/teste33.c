// Teste 33: Geracao Lua para while com incremento pos-fixado
// Entrada em C:
#include <stdio.h>

int main() {
    int i = 0;
    int total = 0;

    while (i < 3) {
        total = total + i;
        i++;
    }

    return total;
}

// Codigo Lua esperado:
// -- Codigo Lua gerado automaticamente pelo compilador C para Lua
//
// function main()
//     local i = 0
//     local total = 0
//     while (i < 3) do
//         do
//             total = (total + i)
//             i = i + 1
//         end
//     end
//     return total
// end
//
// main()
