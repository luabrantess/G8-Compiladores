int x = 0;
while (x < 10) {
    if (x == 5) {
        x = x + 2;
    } else {
        x++;
    }
}

// --- SAIDA ESPERADA NA ARVORE ---
// --- COMANDO ---
//   DECLARACAO: tipo[258] x
//     NUMERO: 0
//   WHILE
//     BINOP: <
//       VARIAVEL: x
//       NUMERO: 10
//   DO
//     IF
//       BINOP: [TOKEN EQ]
//         VARIAVEL: x
//         NUMERO: 5
//     THEN
//       ATRIBUICAO: x =
//         BINOP: +
//           VARIAVEL: x
//           NUMERO: 2
//     ELSE
//       UNOP: [TOKEN INC]
//         VARIAVEL: x