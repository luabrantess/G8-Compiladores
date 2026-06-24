// Teste 13: Diretiva de pre-processador nao suportada
// Entrada invalida: o compilador ignora #include simples, mas nao reconhece #define.

#define LIMITE 10

int main() {
    return LIMITE;
}

// Saida esperada:
// Iniciando analise sintatica...
// Erro lexico na linha 4: #
// A analise falhou devido a erros lexicos.
