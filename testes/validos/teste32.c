// Teste 32: Geracao Lua para if/else e printf
// Entrada em C:
#include <stdio.h>

int main() {
    int nota = 8;

    if (nota >= 7) {
        printf("aprovado\n");
    } else {
        printf("reprovado\n");
    }

    return 0;
}

// Codigo Lua esperado:
// -- Codigo Lua gerado automaticamente pelo compilador C para Lua
//
// function main()
//     local nota = 8
//     if (nota >= 7) then
//         do
//             io.write(string.format("aprovado\n"))
//         end
//     else
//         do
//             io.write(string.format("reprovado\n"))
//         end
//     end
//     return 0
// end
//
// main()
