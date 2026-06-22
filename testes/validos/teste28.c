// Teste 28: Atribuicao entre tipos incompativeis
// Testa verificacao de tipos na tabela de simbolos

#include <stdio.h>
int main() {
int x = 3.14;
return 0;
}
// Saida esperada:
// Analise concluida com sucesso!

// --- TABELA DE SIMBOLOS ---
// Erro semantico na linha 6: atribuicao incompativel para 'x' (esperado int, encontrado float)
// Nome             Tipo         Categoria    Escopo   TamArray   Params     Definida  
// ---------------------------------------------------------------------------------
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
//         DECLARACAO: tipo[258] x
//       FLOAT: 3.140000
//   RETURN
//     NUMERO: 0

// Codigo Lua nao gerado devido a erros semanticos.