# Especificações do Compilador

O compilador que traduz C para Lua atua em uma versão reduzida e não disponibiliza todas as funcionalidades dessas linguagens. Estes são os elementos que podem ser traduzidos:

- **Tipos de Dados**: `int`, `char` e `float`.
- **Condicionais e Repetição**: `if`, `else`, `while` e `for`.
- **Identificadores e Números**: `[a-zA-Z_][a-zA-Z0-9_]`, `[0-9]+` e `[0-9]+"."[0-9]+`.
- **Operadores de Comparação**: `==`, `!=`, `>=`, `<=`, `>` e `<`.
- **Operadores Lógicos**: `&&`, `||` e `!`.
- **Operadores Aritméticos**: `+`, `-`, `*` e `/`.
- **Operador de Atribuição**: `=`.
- **Delimitadores e Pontuação**: `(`, `)`, `{`, `}` e `;`.

As funcionalidades não mencionadas acima não estão disponíveis para a compilação neste projeto.

## Histórico de versões

| Versão | Data       | Autor           | Descrição da Alteração                  | Revisor   |
|--------|-----------|----------------|----------------------------------------|--------------|
| 1.0    | 15/04/2026 | Samara Letícia   | Adição das especificações do compilador | |