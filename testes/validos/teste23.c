// Teste 23: Escopo de bloco - variavel de bloco interno nao visivel no externo
// Testa isolamento de escopo na tabela de simbolos
#include <stdio.h>

int main() {
int x = 0;
    if (x == 0) {
        int y = 99;
        x = y;
    }
    x = y;
    return 0;
}
// Saida esperada:
// Iniciando analise sintatica...
// Analise concluida com sucesso!

// --- TABELA DE SIMBOLOS ---
// Erro semantico na linha 11: variavel 'y' nao declarado(a)
// Nome             Tipo         Categoria    Escopo   TamArray   Params     Definida  
// ---------------------------------------------------------------------------------
// y                int          variavel     2        0          0          -         
// x                int          variavel     1        0          0          -         
// main             int          funcao       0        0          0          sim       
// printf           int          funcao       0        0          variadico  sim       

// Analise semantica encontrou 1 erro(s).

// --- ARVORE SINTATICA GERADA ---
// --- COMANDO ---
// FUNCAO DECLARADA: main
// PARAMETROS:
// CORPO:
//   --- COMANDO ---
//     --- COMANDO ---
//       --- COMANDO ---
//                 DECLARACAO: tipo[258] x
//           NUMERO: 0
//       IF
//         BINOP: ==
//           VARIAVEL: x
//           NUMERO: 0
// THEN
//                 --- COMANDO ---
//                     DECLARACAO: tipo[258] y
//             NUMERO: 99
//         ATRIBUICAO: x =
//           VARIAVEL: y
//     ATRIBUICAO: x =
//       VARIAVEL: y
//   RETURN
//     NUMERO: 0

// Codigo Lua nao gerado devido a erros semanticos.