// Teste 30: Multiplas funcoes declaradas e tabela de simbolos global
// Testa registro e lookup de multiplas funcoes na tabela de simbolos
#include <stdio.h>

int dobrar(int x) {
    return x * 2;
}

int triplicar(int x) {
    return x * 3;
}
int main() {
    int a = dobrar(5);
    int b = triplicar(a);
    return 0;
}

// --- SAIDA ESPERADA ---
// Iniciando analise sintatica...
// Analise concluida com sucesso!

// --- TABELA DE SIMBOLOS ---
// Nome             Tipo         Categoria    Escopo   TamArray   Params     Definida  
// ---------------------------------------------------------------------------------
// b                int          variavel     3        0          0          -         
// a                int          variavel     3        0          0          -         
// x                int          parametro    2        0          0          -         
// x                int          parametro    1        0          0          -         
// main             int          funcao       0        0          0          sim       
// triplicar        int          funcao       0        0          1          sim       
// dobrar           int          funcao       0        0          1          sim       
// printf           int          funcao       0        0          variadico  sim       

// Analise semantica concluida sem erros.

// --- ARVORE SINTATICA GERADA ---
// --- COMANDO ---
//   --- COMANDO ---
//     --- COMANDO ---
//     FUNCAO DECLARADA: dobrar
// PARAMETROS:
//             DECLARACAO: tipo[258] x
// CORPO:
//       RETURN
//         BINOP: *
//           VARIAVEL: x
//           NUMERO: 2
//   FUNCAO DECLARADA: triplicar
// PARAMETROS:
//         DECLARACAO: tipo[258] x
// CORPO:
//     RETURN
//       BINOP: *
//         VARIAVEL: x
//         NUMERO: 3
// FUNCAO DECLARADA: main
// PARAMETROS:
// CORPO:
//   --- COMANDO ---
//     --- COMANDO ---
//             DECLARACAO: tipo[258] a
//         CHAMADA DE FUNCAO: dobrar()
//                     NUMERO: 5
//         DECLARACAO: tipo[258] b
//       CHAMADA DE FUNCAO: triplicar()
//                 VARIAVEL: a
//   RETURN
//     NUMERO: 0

// Codigo Lua gerado em: saida.lua

// Codigo lua:
// -- Codigo Lua gerado automaticamente pelo compilador C para Lua

// function dobrar(x)
//     return (x * 2)
// end

// function triplicar(x)
//     return (x * 3)
// end

// function main()
//     local a = dobrar(5)
//     local b = triplicar(a)
//     return 0
// end

// main()
