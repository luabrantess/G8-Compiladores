// Teste 21: Redeclaracao de variavel no mesmo escopo
// Testa se a tabela de simbolos detecta declaracao duplicada

int a = 5;
int a = 10;

// Saida esperada:
//Iniciando analise sintatica...
//Analise concluida!
//--- TABELA DE SIMBOLOS ---
//Erro semantico: 'a' ja foi declarado neste escopo
//Nome             Tipo         Categoria    Escopo   TamArray   Params    
//----------------------------------------------------------------------
//a                int          variavel     0        0          0         
//
//Analise semantica encontrou 1 erro(s).

//--- ARVORE SINTATICA GERADA ---
//--- COMANDO ---
//DECLARACAO: tipo[258] a
//NUMERO: 5
//DECLARACAO: tipo[258] a
//  NUMERO: 10