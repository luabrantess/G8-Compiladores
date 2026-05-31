// Teste 23: Escopo de bloco - variavel de bloco interno nao visivel no externo
// Testa isolamento de escopo na tabela de simbolos

int x = 0;
if (x == 0) {
    int y = 99;
    x = y;
}
x = y;

// Saida esperada:
// Iniciando analise sintatica...
// Analise concluida com sucesso!

// --- TABELA DE SIMBOLOS ---
// Erro semantico: variavel 'y' nao declarado(a)
// Nome             Tipo         Categoria    Escopo   TamArray   Params    
// ----------------------------------------------------------------------
// y                int          variavel     1        0          0         
// x                int          variavel     0        0          0         

// Analise semantica encontrou 1 erro(s).

// --- ARVORE SINTATICA GERADA ---
// --- COMANDO ---
//   --- COMANDO ---
//     DECLARACAO: tipo[258] x
//       NUMERO: 0
//   IF
//     BINOP: [TOKEN 271]
//       VARIAVEL: x
//       NUMERO: 0
// THEN
//         --- COMANDO ---
//       DECLARACAO: tipo[258] y
//         NUMERO: 99
//     ATRIBUICAO: x =
//       VARIAVEL: y
// ATRIBUICAO: x =
//   VARIAVEL: y