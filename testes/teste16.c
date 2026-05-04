int a = 10 + 5 * 2;
int b = (10 + 5) * 2;
int c = -a;

// --- SAIDA ESPERADA NA ARVORE ---
// --- COMANDO ---
//   --- COMANDO ---
//     DECLARACAO: tipo[258] a
//       BINOP: +
//         NUMERO: 10
//         BINOP: *
//           NUMERO: 5
//           NUMERO: 2
//     DECLARACAO: tipo[258] b
//       BINOP: *
//         BINOP: +
//           NUMERO: 10
//           NUMERO: 5
//         NUMERO: 2
//   DECLARACAO: tipo[258] c
//     UNOP: -
//       VARIAVEL: a