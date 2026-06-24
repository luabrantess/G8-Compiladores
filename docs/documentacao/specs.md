# Especificações do Compilador

O compilador que traduz C para Lua atua em uma versão reduzida e não disponibiliza todas as funcionalidades dessas linguagens. Estes são os elementos que podem ser traduzidos:

- **Tipos de Dados**: `int`, `char`, `double`, `array`, `string` e `float`.
- **Condicionais e Repetição**: `if`, `else`, `while` e `for`.
- **Identificadores e Números**: `[a-zA-Z_][a-zA-Z0-9_]`, `[0-9]+` e `[0-9]+"."[0-9]+`.
- **Operadores de Comparação**: `==`, `!=`, `>=`, `<=`, `>` e `<`.
- **Operadores Lógicos**: `&&`, `||` e `!`.
- **Operadores Aritméticos**: `+`, `-`, `*` e `/`.
- **Operador de Atribuição**: `=`.
- **Delimitadores e Pontuação**: `(`, `)`, `{`, `}` e `;`.
- **Outros**: `void`, `function`, `return`, `printf`.

As funcionalidades não mencionadas acima não estão disponíveis para a compilação neste projeto.

## Cobertura dos Testes

### Testes válidos (resultado obtido = resultado esperado)

| Teste | Funcionalidade validada | Componente exercitado | Resultado esperado |
|-------|-------------------------|-----------------------|--------------------|
| teste1 | Condicional simples com chamada de `printf` | Análise sintática / Análise semântica / Geração de Lua | Sem erros + código Lua gerado |
| teste2 | Número negativo com ponto flutuante em laço `while` | Análise léxica / Análise sintática / Geração de Lua | Sem erros + código Lua gerado |
| teste5 | Condicional `if/else` com operador `!=` | Análise sintática / Análise semântica / Geração de Lua | Sem erros + código Lua gerado |
| teste6 | Declarações e aritmética básica com `int` e `float` | Análise sintática / Análise semântica / Geração de Lua | Sem erros + código Lua gerado |
| teste8 | Condicionais aninhadas dentro de repetição `while` | Análise sintática / Geração de Lua | Sem erros + código Lua gerado |
| teste9 | Laço `for` com expressões relacionais e operador lógico `&&` | Análise sintática / Geração de Lua | Sem erros + código Lua gerado |
| teste11 | Condicional simples com operador de igualdade `==` dentro de `main` | Análise sintática / Análise semântica / Geração de Lua | Sem erros + código Lua gerado |
| teste12 | Laço `for` com inicialização, condição e atualização por atribuição | Análise sintática / Geração de Lua | Sem erros + código Lua gerado |
| teste14 | Declaração de variável `float` com valor decimal | Análise léxica / Análise sintática / Geração de Lua | Sem erros + código Lua gerado |
| teste16 | Precedência de operadores aritméticos e uso de menos unário | Análise sintática / AST / Geração de Lua | Sem erros + código Lua gerado |
| teste17 | Laço `while` com `if/else` e incremento pós-fixado | Análise sintática / AST / Geração de Lua | Sem erros + código Lua gerado |
| teste18 | Declaração e chamada de função com parâmetros | Tabela de símbolos / Análise semântica / Geração de Lua | Sem erros + código Lua gerado |
| teste19 | Expressão condicional com operadores relacionais e operador lógico `&&` | Análise sintática / Análise semântica / Geração de Lua | Sem erros + código Lua gerado |
| teste21 | Redeclaração de variável no mesmo escopo | Tabela de símbolos / Análise semântica | Erro semântico: `'a' ja foi declarado neste escopo` |
| teste23 | Isolamento de escopo de bloco (variável interna não visível externamente) | Tabela de símbolos (escopos) / Análise semântica | Erro semântico: variável `'y'` não declarada no escopo externo |
| teste25 | Chamada de função não declarada | Tabela de símbolos (lookup) / Análise semântica | Erro semântico: função `'somar'` não declarada |
| teste26 | Função com tipo de retorno e chamada com tipos compatíveis | Análise semântica (tipos) / Geração de Lua | Sem erros + código Lua gerado |
| teste27 | Número incorreto de argumentos na chamada (aridade) | Tabela de símbolos / Análise semântica | Erro semântico: `'dobrar'` esperava 1 argumento, recebeu 2 |
| teste28 | Atribuição entre tipos incompatíveis (`int = float`) | Análise semântica (verificação de tipos) | Erro semântico: atribuição incompatível para `'x'` |
| teste29 | Parâmetros de função acessíveis dentro do corpo | Tabela de símbolos (parâmetros/escopo) / Geração de Lua | Sem erros + código Lua gerado |
| teste30 | Registro e lookup de múltiplas funções no escopo global | Tabela de símbolos (global) / Geração de Lua | Sem erros + código Lua gerado |
| teste31 | Tradução de função com retorno e expressão aritmética | Geração de Lua (funções / retorno / expressões) | Função, variáveis locais e retorno traduzidos corretamente para Lua |
| teste32 | Tradução de condicional `if/else` com chamada de `printf` | Geração de Lua (condicionais / entrada e saída) | `if/else` traduzido para `then/else/end` e `printf` para `io.write(string.format(...))` |
| teste33 | Tradução de laço `while` com incremento pós-fixado | Geração de Lua (repetição / incremento) | `while` traduzido para `while ... do` e `i++` para `i = i + 1` |
| teste34 | Tradução de laço `for` para bloco com `while` | Geração de Lua (repetição / atribuição) | Inicialização, condição e atualização do `for` traduzidas para um bloco Lua com `while` |
| teste35 | Tradução de array, acesso e atribuição por índice | Geração de Lua (arrays) | Array C traduzido para tabela Lua com índices iniciados em zero |

### Testes inválidos (resultado obtido ≠ resultado esperado)

| Teste | Funcionalidade validada | Componente exercitado | Resultado esperado | Motivo da falha |
|-------|-------------------------|-----------------------|--------------------|-----------------|
| teste3 | Erros sintáticos diversos | Análise sintática | Erro sintático: `syntax error` | Falha no parser porque o `while` está sem parênteses: `while x > 0 { }` |
| teste4 | Lista e laço `for-in` | Análise sintática | Erro sintático: `syntax error` | A gramática não reconhece `for-in`: `for i in x { ... }` |
| teste7 | Falta de ponto e vírgula em declaração | Análise sintática | Erro sintático: `syntax error` | Falta `;` após `int y = 20` |
| teste10 | Caractere inválido e `if` sem parênteses | Análise léxica / Análise sintática | Erro léxico em `@` + erro sintático | Falha no lexer por causa de `@` e depois no parser pela expressão quebrada |
| teste13 | Diretiva de pré-processador não suportada (`#define`) | Análise léxica | Erro léxico em `#` | O lexer ignora apenas diretivas simples de `#include`, mas não reconhece `#define` |
| teste15 | Declaração de variável sem ponto e vírgula | Análise sintática | Erro sintático: `syntax error` | Falta `;` após `int x = -2` |
| teste20 | Uso de variável sem declaração prévia | Tabela de símbolos / Análise semântica | Erro semântico: variável `'x'` não declarada | A atribuição usa `x` sem declaração anterior no escopo |
| teste22 | Aviso de variável declarada mas nunca utilizada | Análise semântica (avisos) | Aviso semântico: variável `'x'` declarada mas nunca utilizada | Verificação não implementada — nenhum aviso emitido |
| teste24 | Aviso de *shadowing* (variável interna oculta a externa) | Tabela de símbolos (escopos aninhados) / Análise semântica | Aviso semântico de *shadowing* da variável `'x'` | Verificação não implementada — aviso não emitido |

## Histórico de versões

| Versão | Data       | Autor           | Descrição da Alteração                  | Revisor   |
|--------|-----------|----------------|----------------------------------------|--------------|
| 1.0    | 15/04/2026 | Samara Letícia   | Adição das especificações do compilador | Ana Luiza |
| 2.0    | 22/06/2026 | Samara Letícia   | Adição da Cobertura dos Testes | Ana Luiza |
| 3.0    | 23/06/2026 | Ana Luiza   | Adição da Cobertura dos Testes |  |
| 4.0    | 24/06/2026 | Bruno Henryque   | Revisão, organização e cobertura dos testes 11 a 20 |  |
