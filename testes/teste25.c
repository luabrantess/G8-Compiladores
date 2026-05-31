// Teste 25: Chamada de funcao nao declarada
// Testa se a tabela de simbolos detecta chamada a funcao inexistente

int resultado = somar(3, 4);

// Saida esperada:
// Iniciando analise sintatica...
// Analise concluida com sucesso!

// --- TABELA DE SIMBOLOS ---
// Erro semantico: funcao 'somar' nao declarado(a)
// Nome             Tipo         Categoria    Escopo   TamArray   Params    
// ----------------------------------------------------------------------
// resultado        int          variavel     0        0          0         

// Analise semantica encontrou 1 erro(s).

// --- ARVORE SINTATICA GERADA ---
// DECLARACAO: tipo[258] resultado
//   CHAMADA DE FUNCAO: somar()
//         NUMERO: 3
//         NUMERO: 4
