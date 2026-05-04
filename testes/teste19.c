int a = 1;
int b = 0;
if (a > 0 && b != 1) {
    int c = a + b;
}

// --- SAIDA ESPERADA NA ARVORE ---
// --- COMANDO ---
//   --- COMANDO ---
//     DECLARACAO: tipo[258] a
//       NUMERO: 1
//     DECLARACAO: tipo[258] b
//       NUMERO: 0
//   IF
//     BINOP: [TOKEN AND]
//       BINOP: >
//         VARIAVEL: a
//         NUMERO: 0
//       BINOP: [TOKEN NEQ]
//         VARIAVEL: b
//         NUMERO: 1
//   THEN
//     DECLARACAO: tipo[258] c
//       BINOP: +
//         VARIAVEL: a
//         VARIAVEL: b