// Teste 22: Variavel declarada mas nunca utilizada
// Testa se a tabela de simbolos emite aviso de variavel nao utilizada
// Resultado: Essa função não foi implementada no compilador. 

int main() {
    int x = 42;
    int y = 7;
    y = y + 1;

    return 0;
}


// Saida esperada:
// Iniciando analise sintatica...
// Analise concluida com sucesso!
// Aviso semantico na linha 3: Variavel 'x' declarada mas nunca utilizada.
// --- TABELA DE SIMBOLOS ---
// Nome             Tipo         Categoria    Escopo   TamArray   Params     Definida  
// ---------------------------------------------------------------------------------
// y                int          variavel     1        0          0          -         
// x                int          variavel     1        0          0          -         
// main             int          funcao       0        0          0          sim       
// printf           int          funcao       0        0          variadico  sim       

// Analise semantica concluida sem erros.

// --- ARVORE SINTATICA GERADA ---
// --- COMANDO ---
// FUNCAO DECLARADA: main
// PARAMETROS:
// CORPO:
//   --- COMANDO ---
//     --- COMANDO ---
//       --- COMANDO ---
//                 DECLARACAO: tipo[258] x
//           NUMERO: 42
//             DECLARACAO: tipo[258] y
//         NUMERO: 7
//     ATRIBUICAO: y =
//       BINOP: +
//         VARIAVEL: y
//         NUMERO: 1
//   RETURN
//     NUMERO: 0

// Codigo Lua gerado em: saida.lua

// Codigo: 

// -- Codigo Lua gerado automaticamente pelo compilador C para Lua

// function main()
//     local x = 42
//     local y = 7
//     y = (y + 1)
//     return 0
// end

// main()
