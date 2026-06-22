// Teste 29: Parametros de funcao registrados corretamente na tabela de simbolos
// Testa se os parametros sao acessiveis dentro do corpo da funcao

#include <stdio.h>

int subtrair(int a, int b) {
    int resultado = a - b;
    return resultado;
}

int main() {

    int r = subtrair(5, 3);
    return 0;
}

// Saída esperada:
// Iniciando analise sintatica...
// Analise concluida com sucesso!

// --- TABELA DE SIMBOLOS ---
// Nome             Tipo         Categoria    Escopo   TamArray   Params     Definida  
// ---------------------------------------------------------------------------------
// r                int          variavel     2        0          0          -         
// resultado        int          variavel     1        0          0          -         
// b                int          parametro    1        0          0          -         
// a                int          parametro    1        0          0          -         
// main             int          funcao       0        0          0          sim       
// subtrair         int          funcao       0        0          2          sim       
// printf           int          funcao       0        0          variadico  sim       

// Analise semantica concluida sem erros.

// --- ARVORE SINTATICA GERADA ---
// --- COMANDO ---
//   --- COMANDO ---
//   FUNCAO DECLARADA: subtrair
// PARAMETROS:
//         DECLARACAO: tipo[258] a
//         DECLARACAO: tipo[258] b
// CORPO:
//     --- COMANDO ---
//             DECLARACAO: tipo[258] resultado
//         BINOP: -
//           VARIAVEL: a
//           VARIAVEL: b
//     RETURN
//       VARIAVEL: resultado
// FUNCAO DECLARADA: main
// PARAMETROS:
// CORPO:
//   --- COMANDO ---
//         DECLARACAO: tipo[258] r
//       CHAMADA DE FUNCAO: subtrair()
//                 NUMERO: 5
//                 NUMERO: 3
//   RETURN
//     NUMERO: 0

// Codigo Lua gerado em: saida.lua

// Codigo em lua:
// function subtrair(a, b)
//     local resultado = (a - b)
//     return resultado
// end

// function main()
//     local r = subtrair(5, 3)
//     return 0
// end

// main()