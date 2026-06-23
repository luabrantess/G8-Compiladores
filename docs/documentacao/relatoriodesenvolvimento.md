# Relatório de Desenvolvimento

## 6 de abril — Definição das Regras do Flex

Foram estabelecidas as regras léxicas para o analisador, contemplando os seguintes elementos da linguagem:

* Ignorar comentários
- **Tipos de Dados**: `int`, `double`, `string`, `array`, `char` e `float`.
- **Condicionais e Repetição**: `if`, `else`, `while` e `for`.
- **Identificadores e Números**: `[a-zA-Z_][a-zA-Z0-9_]`, `[0-9]+` e `[0-9]+"."[0-9]+`.
- **Operadores de Comparação**: `==`, `!=`, `>=`, `<=`, `>` e `<`.
- **Operadores Lógicos**: `&&`, `||` e `!`.
- **Operadores Aritméticos**: `+`, `-`, `*` e `/`.
- **Operador de Atribuição**: `=`.
- **Delimitadores e Pontuação**: `(`, `)`, `{`, `}` e `;`.
- **Outros**: `void`, `function`, `return` e `printf`.

---

## 10 de abril — Testes de Software

Foram realizados testes envolvendo:

* Estruturas condicionais (`if`)
* Declaração de tipos
* Operadores
* Delimitadores

O arquivo `lexer.l` foi validado com sucesso, atendendo aos requisitos definidos.

---

## 11 de abril — Configuração do Ambiente

* Configuração do `Makefile` para automação da compilação
* Configuração do arquivo `parser.y` para análise sintática

---

## 15 de abril — Ajustes na Documentação

* Edição da página no GitHub Pages para melhorar a apresentação
* Padronização visual com foco em um aspecto mais profissional

## Histórico de versões

| Versão | Data       | Autor           | Descrição da Alteração                  | Revisor   |
|--------|-----------|----------------|----------------------------------------|--------------|
| 1.0    | 15/04/2026 | [Ana Luiza](https://github.com/luabrantess)  | Adição do planejamento da sprint 1 | |