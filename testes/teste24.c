// Teste 24: Shadowing - variavel interna com mesmo nome da externa
// Testa se a tabela de simbolos trata corretamente escopos aninhados

int x = 1;
if (x > 0) {
    int x = 2;
    x = x + 1;
}

// Saida esperada:
//Iniciando analise sintatica...
//Analise concluida!
//Aviso semantico: variavel 'x' no escopo interno oculta variavel do escopo externo

// Saida recebida:

// Iniciando analise sintatica...
// Analise concluida com sucesso!

// --- TABELA DE SIMBOLOS ---
// Nome             Tipo         Categoria    Escopo   TamArray   Params    
// ----------------------------------------------------------------------
// x                int          variavel     1        0          0         
// x                int          variavel     0        0          0         

// Analise semantica concluida sem erros.

// --- ARVORE SINTATICA GERADA ---
// --- COMANDO ---
//   DECLARACAO: tipo[258] x
//     NUMERO: 1
// IF
//   BINOP: >
//     VARIAVEL: x
//     NUMERO: 0
// THEN
//     --- COMANDO ---
//     DECLARACAO: tipo[258] x
//       NUMERO: 2
//   ATRIBUICAO: x =
//     BINOP: +
//       VARIAVEL: x
//       NUMERO: 1

// Teste não passou, o analisador semantico não emitiu o aviso de variavel 'x' no escopo interno oculta variavel do escopo externo. (shadowing)