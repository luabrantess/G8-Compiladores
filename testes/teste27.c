// Teste 27: Numero incorreto de argumentos na chamada de funcao
// Testa verificacao de aridade na tabela de simbolos

#include <stdio.h>

int dobrar(int x) {
    return x * 2;
}


int main() {

    int r = dobrar(3, 7);

    return 0;
}

// Saida esperada:
// Iniciando analise sintatica...
// Analise concluida com sucesso!

// --- TABELA DE SIMBOLOS ---
// Erro semantico na linha 13: funcao 'dobrar' esperava 1 argumento(s), mas recebeu 2
// Nome             Tipo         Categoria    Escopo   TamArray   Params     Definida  
// ---------------------------------------------------------------------------------
// r                int          variavel     2        0          0          -         
// x                int          parametro    1        0          0          -         
// main             int          funcao       0        0          0          sim       
// dobrar           int          funcao       0        0          1          sim       
// printf           int          funcao       0        0          variadico  sim       

// Analise semantica encontrou 1 erro(s).

// --- ARVORE SINTATICA GERADA ---
// --- COMANDO ---
//   --- COMANDO ---
//   FUNCAO DECLARADA: dobrar
// PARAMETROS:
//         DECLARACAO: tipo[258] x
// CORPO:
//     RETURN
//       BINOP: *
//         VARIAVEL: x
//         NUMERO: 2
// FUNCAO DECLARADA: main
// PARAMETROS:
// CORPO:
//   --- COMANDO ---
//         DECLARACAO: tipo[258] r
//       CHAMADA DE FUNCAO: dobrar()
//                 NUMERO: 3
//                 NUMERO: 7
//   RETURN
//     NUMERO: 0

// Codigo Lua nao gerado devido a erros semanticos.