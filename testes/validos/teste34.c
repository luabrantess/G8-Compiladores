// Teste 34: Geracao Lua para for convertido em bloco com while
// Entrada em C:
#include <stdio.h>

int main() {
    int i;
    int total = 0;

    for (i = 0; i < 4; i = i + 1) {
        total = total + i;
    }

    return total;
}

// Codigo Lua esperado:
// -- Codigo Lua gerado automaticamente pelo compilador C para Lua
//
// function main()
//     local i = 0
//     local total = 0
//     do
//         i = 0
//         while (i < 4) do
//             do
//                 total = (total + i)
//             end
//             i = (i + 1)
//         end
//     end
//     return total
// end
//
// main()
