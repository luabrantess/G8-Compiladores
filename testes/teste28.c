// Teste 28: Atribuicao entre tipos incompativeis
// Testa verificacao de tipos na tabela de simbolos

int x = 3.14;

// Saida esperada:
// Iniciando analise sintatica...
// Analise concluida com sucesso!

// --- TABELA DE SIMBOLOS ---
// Erro semantico: atribuicao incompativel para 'x' (esperado int, encontrado float)
// Nome             Tipo         Categoria    Escopo   TamArray   Params    
// ----------------------------------------------------------------------
// x                int          variavel     0        0          0         

// Analise semantica encontrou 1 erro(s).

// --- ARVORE SINTATICA GERADA ---
// DECLARACAO: tipo[258] x
//   FLOAT: 3.140000