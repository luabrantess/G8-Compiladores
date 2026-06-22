// Teste 25: Chamada de funcao nao declarada
// Testa se a tabela de simbolos detecta chamada a funcao inexistente
#include <stdio.h>

int main() {
    int resultado = somar(3, 4);
    return 0;
}

// Saida esperada:
// Iniciando analise sintatica...
// Analise concluida com sucesso!

// --- TABELA DE SIMBOLOS ---
// Erro semantico na linha 6: funcao 'somar' nao declarado(a)
// Nome             Tipo         Categoria    Escopo   TamArray   Params     Definida  
// ---------------------------------------------------------------------------------
// resultado        int          variavel     1        0          0          -         
// main             int          funcao       0        0          0          sim       
// printf           int          funcao       0        0          variadico  sim       

// Analise semantica encontrou 1 erro(s).

// --- ARVORE SINTATICA GERADA ---
// --- COMANDO ---
// FUNCAO DECLARADA: main
// PARAMETROS:
// CORPO:
//   --- COMANDO ---
//         DECLARACAO: tipo[258] resultado
//       CHAMADA DE FUNCAO: somar()
//                 NUMERO: 3
//                 NUMERO: 4
//   RETURN
//     NUMERO: 0

// Codigo Lua nao gerado devido a erros semanticos.