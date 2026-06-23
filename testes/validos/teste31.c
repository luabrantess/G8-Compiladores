// Teste 31: Geracao Lua para funcao com retorno e expressao aritmetica
// Entrada em C:
#include <stdio.h>

int somar_dobro(int a, int b) {
    int soma = a + b;
    return soma * 2;
}

int main() {
    int resultado = somar_dobro(3, 4);
    return resultado;
}

// Codigo Lua esperado:
// -- Codigo Lua gerado automaticamente pelo compilador C para Lua
//
// function somar_dobro(a, b)
//     local soma = (a + b)
//     return (soma * 2)
// end
//
// function main()
//     local resultado = somar_dobro(3, 4)
//     return resultado
// end
//
// main()
