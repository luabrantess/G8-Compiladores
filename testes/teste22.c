// Teste 22: Variavel declarada mas nunca utilizada
// Testa se a tabela de simbolos emite aviso de variavel nao utilizada

int x = 42;
int y = 7;
y = y + 1;

// Saida esperada:
// Iniciando analise sintatica...
// Analise concluida com sucesso!

//Aviso semantico: variavel 'x' declarada mas nunca utilizada

// --- TABELA DE SIMBOLOS ---
// Nome             Tipo         Categoria    Escopo   TamArray   Params    
// ----------------------------------------------------------------------
// y                int          variavel     0        0          0         
// x                int          variavel     0        0          0         

// Analise semantica concluida sem erros.

// --- ARVORE SINTATICA GERADA ---
// --- COMANDO ---
//   --- COMANDO ---
//     DECLARACAO: tipo[258] x
//       NUMERO: 42
//   DECLARACAO: tipo[258] y
//     NUMERO: 7
// ATRIBUICAO: y =
//   BINOP: +
//     VARIAVEL: y
//     NUMERO: 1

//Teste não passou, o analisador semantico não emitiu o aviso de variavel 'x' declarada mas nunca utilizada.