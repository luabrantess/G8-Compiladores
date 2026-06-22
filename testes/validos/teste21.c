// Teste 21: Redeclaracao de variavel no mesmo escopo
// Testa se a tabela de simbolos detecta declaracao duplicada
#include <stdio.h>

int main() {
    int a = 5;
    int a = 10;
    return 0;
}

// Saida esperada:

// Iniciando analise sintatica...
// Analise concluida com sucesso!

// --- TABELA DE SIMBOLOS ---
// Erro semantico na linha 7: 'a' ja foi declarado neste escopo
// Nome             Tipo         Categoria    Escopo   TamArray   Params     Definida  
// ---------------------------------------------------------------------------------
// a                int          variavel     1        0          0          -         
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
//             DECLARACAO: tipo[258] a
//         NUMERO: 5
//         DECLARACAO: tipo[258] a
//       NUMERO: 10
//   RETURN
//     NUMERO: 0

// Codigo Lua nao gerado devido a erros semanticos.