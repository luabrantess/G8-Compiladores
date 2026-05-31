// Teste 27: Numero incorreto de argumentos na chamada de funcao
// Testa verificacao de aridade na tabela de simbolos

int dobrar(int x) {
    return x * 2;
}

int r = dobrar(3, 7);

// Saida esperada:
// Iniciando analise sintatica...
// Analise concluida com sucesso!

// --- TABELA DE SIMBOLOS ---
// Erro semantico: funcao 'dobrar' esperava 1 argumento(s), mas recebeu 2
// Nome             Tipo         Categoria    Escopo   TamArray   Params    
// ----------------------------------------------------------------------
// r                int          variavel     0        0          0         
// x                int          parametro    1        0          0         
// dobrar           int          funcao       0        0          1         

// Analise semantica encontrou 1 erro(s).

// --- ARVORE SINTATICA GERADA ---
// --- COMANDO ---
//   FUNCAO DECLARADA: dobrar
// PARAMETROS:
//         DECLARACAO: tipo[258] x
// CORPO:
//     RETURN
//       BINOP: *
//         VARIAVEL: x
//         NUMERO: 2
// DECLARACAO: tipo[258] r
//   CHAMADA DE FUNCAO: dobrar()
//         NUMERO: 3
//         NUMERO: 7