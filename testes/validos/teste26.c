// Teste 26: Funcao com retorno e verificacao de tipo de retorno
// Testa registro de funcao na tabela de simbolos com tipo de retorno

#include <stdio.h>


float calcular(int a, float b) {
    return a + b;
}
int main() {
float r = calcular(2, 3.5);
return 0;
}

// Analise concluida com sucesso!

// --- TABELA DE SIMBOLOS ---
// Nome             Tipo         Categoria    Escopo   TamArray   Params     Definida  
// ---------------------------------------------------------------------------------
// r                float        variavel     2        0          0          -         
// b                float        parametro    1        0          0          -         
// a                int          parametro    1        0          0          -         
// main             int          funcao       0        0          0          sim       
// calcular         float        funcao       0        0          2          sim       
// printf           int          funcao       0        0          variadico  sim       

// Analise semantica concluida sem erros.

// --- ARVORE SINTATICA GERADA ---
// --- COMANDO ---
//   --- COMANDO ---
//   FUNCAO DECLARADA: calcular
// PARAMETROS:
//         DECLARACAO: tipo[258] a
//         DECLARACAO: tipo[260] b
// CORPO:
//     RETURN
//       BINOP: +
//         VARIAVEL: a
//         VARIAVEL: b
// FUNCAO DECLARADA: main
// PARAMETROS:
// CORPO:
//   --- COMANDO ---
//         DECLARACAO: tipo[260] r
//       CHAMADA DE FUNCAO: calcular()
//                 NUMERO: 2
//                 FLOAT: 3.500000
//   RETURN
//     NUMERO: 0

// Codigo Lua gerado em: saida.lua


// Codigo em Lua:
// function calcular(a, b)
//     return (a + b)
// end

// function main()
//     local r = calcular(2, 3.5)
//     return 0
// end

// main()