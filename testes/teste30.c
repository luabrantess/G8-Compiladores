// Teste 30: Multiplas funcoes declaradas e tabela de simbolos global
// Testa registro e lookup de multiplas funcoes na tabela de simbolos

int dobrar(int x) {
    return x * 2;
}

int triplicar(int x) {
    return x * 3;
}

int a = dobrar(5);
int b = triplicar(a);

// --- SAIDA ESPERADA NA ARVORE ---
// --- COMANDO ---
//   FUNCAO DECLARADA: dobrar
//   PARAMETROS:
//     DECLARACAO: tipo[258] x
//   CORPO:
//     RETURN
//       BINOP: *
//         VARIAVEL: x
//         NUMERO: 2
//   FUNCAO DECLARADA: triplicar
//   PARAMETROS:
//     DECLARACAO: tipo[258] x
//   CORPO:
//     RETURN
//       BINOP: *
//         VARIAVEL: x
//         NUMERO: 3
//   DECLARACAO: tipo[258] a
//     CHAMADA DE FUNCAO: dobrar()
//       NUMERO: 5
//   DECLARACAO: tipo[258] b
//     CHAMADA DE FUNCAO: triplicar()
//       VARIAVEL: a
