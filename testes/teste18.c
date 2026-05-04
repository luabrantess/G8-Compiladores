int multiplicar(int x, int y) {
    return x * y;
}

int resultado = multiplicar(4, 5);

// --- SAIDA ESPERADA NA ARVORE ---
// --- COMANDO ---
//   FUNCAO DECLARADA: multiplicar
//   PARAMETROS:
//     DECLARACAO: tipo[258] x
//     DECLARACAO: tipo[258] y
//   CORPO:
//     RETURN
//       BINOP: *
//         VARIAVEL: x
//         VARIAVEL: y
//   DECLARACAO: tipo[258] resultado
//     CHAMADA DE FUNCAO: multiplicar()
//       NUMERO: 4
//       NUMERO: 5