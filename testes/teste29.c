// Teste 29: Parametros de funcao registrados corretamente na tabela de simbolos
// Testa se os parametros sao acessiveis dentro do corpo da funcao

int subtrair(int a, int b) {
    int resultado = a - b;
    return resultado;
}

// --- SAIDA ESPERADA NA ARVORE ---
// --- COMANDO ---
//   FUNCAO DECLARADA: subtrair
//   PARAMETROS:
//     DECLARACAO: tipo[258] a
//     DECLARACAO: tipo[258] b
//   CORPO:
//     DECLARACAO: tipo[258] resultado
//       BINOP: -
//         VARIAVEL: a
//         VARIAVEL: b
//     RETURN
//       VARIAVEL: resultado
