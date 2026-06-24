// Teste 17: While com if/else e incremento pos-fixado
// Entrada valida em C reduzido.

int main() {
    int x = 0;

    while (x < 10) {
        if (x == 5) {
            x = x + 2;
        } else {
            x++;
        }
    }

    return x;
}

// Saida esperada:
// Iniciando analise sintatica...
// Analise concluida com sucesso!
// Codigo Lua gerado em: saida.lua
