// Teste 26: Funcao com retorno e verificacao de tipo de retorno
// Testa registro de funcao na tabela de simbolos com tipo de retorno

float calcular(int a, float b) {
    return a + b;
}

float r = calcular(2, 3.5);

// --- SAIDA ESPERADA NA ARVORE ---
// --- COMANDO ---
//   FUNCAO DECLARADA: calcular
//   PARAMETROS:
//     DECLARACAO: tipo[258] a
//     DECLARACAO: tipo[260] b
//   CORPO:
//     RETURN
//       BINOP: +
//         VARIAVEL: a
//         VARIAVEL: b
//   DECLARACAO: tipo[260] r
//     CHAMADA DE FUNCAO: calcular()
//       NUMERO: 2
//       NUMERO: 3.5
