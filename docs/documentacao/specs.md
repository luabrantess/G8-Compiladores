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

## Cobertura dos Testes

### Testes válidos (resultado obtido = resultado esperado)

| Teste | Funcionalidade validada | Componente exercitado | Resultado esperado |
|-------|-------------------------|-----------------------|--------------------|
| teste21 | Redeclaração de variável no mesmo escopo | Tabela de símbolos / Análise semântica | Erro semântico: `'a' ja foi declarado neste escopo` |
| teste23 | Isolamento de escopo de bloco (variável interna não visível externamente) | Tabela de símbolos (escopos) / Análise semântica | Erro semântico: variável `'y'` não declarada no escopo externo |
| teste25 | Chamada de função não declarada | Tabela de símbolos (lookup) / Análise semântica | Erro semântico: função `'somar'` não declarada |
| teste26 | Função com tipo de retorno e chamada com tipos compatíveis | Análise semântica (tipos) / Geração de Lua | Sem erros + código Lua gerado |
| teste27 | Número incorreto de argumentos na chamada (aridade) | Tabela de símbolos / Análise semântica | Erro semântico: `'dobrar'` esperava 1 argumento, recebeu 2 |
| teste28 | Atribuição entre tipos incompatíveis (`int = float`) | Análise semântica (verificação de tipos) | Erro semântico: atribuição incompatível para `'x'` |
| teste29 | Parâmetros de função acessíveis dentro do corpo | Tabela de símbolos (parâmetros/escopo) / Geração de Lua | Sem erros + código Lua gerado |
| teste30 | Registro e lookup de múltiplas funções no escopo global | Tabela de símbolos (global) / Geração de Lua | Sem erros + código Lua gerado |

### Testes inválidos (resultado obtido ≠ resultado esperado)

| Teste | Funcionalidade validada | Componente exercitado | Resultado esperado | Motivo da falha |
|-------|-------------------------|-----------------------|--------------------|-----------------|
| teste22 | Aviso de variável declarada mas nunca utilizada | Análise semântica (avisos) | Aviso semântico: variável `'x'` declarada mas nunca utilizada | Verificação não implementada — nenhum aviso emitido |
| teste24 | Aviso de *shadowing* (variável interna oculta a externa) | Tabela de símbolos (escopos aninhados) / Análise semântica | Aviso semântico de *shadowing* da variável `'x'` | Verificação não implementada — aviso não emitido |

## Histórico de versões

| Versão | Data       | Autor           | Descrição da Alteração                  | Revisor   |
|--------|-----------|----------------|----------------------------------------|--------------|
| 1.0    | 15/04/2026 | Samara Letícia   | Adição das especificações do compilador | |
| 2.0    | 22/06/2026 | Samara Letícia   | Adição da Cobertura dos Testes | |