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

---

## 24 de junho — Revisão dos Testes 11 a 20

Foi realizada a revisão dos testes atribuídos ao intervalo `teste11` a `teste20`, com foco em compatibilidade com a versão atual do compilador C para Lua.

Durante a revisão, foram encontradas as seguintes dificuldades:

- Alguns testes antigos possuíam comandos como `if`, `for` e `while` diretamente no escopo global. Após as mudanças no analisador sintático, esses comandos passaram a ser aceitos apenas dentro de blocos de função, como `int main() { ... }`.
- O teste relacionado a `#include` ficou incompatível com o comportamento atual do analisador léxico, pois o lexer passou a reconhecer e ignorar diretivas simples de inclusão.
- Os testes `11` a `20` ainda não estavam organizados nas pastas de casos válidos e inválidos.
- A tabela de cobertura ainda não relacionava esses testes com as funcionalidades validadas.

As soluções adotadas foram:

- Ajustar os testes válidos para usarem estrutura de função, mantendo os mesmos objetivos de validação: condicionais, laços, operadores, declaração de tipos, funções e chamadas.
- Alterar o teste de diretiva de pré-processador para usar `#define`, que continua fora do subconjunto de C reconhecido pelo compilador.
- Manter testes inválidos específicos para erro léxico, erro sintático e erro semântico.
- Organizar os arquivos nas pastas `testes/validos` e `testes/invalidos`.
- Atualizar a tabela de cobertura em `specs.md`, adicionando a relação dos testes `11` a `20` com as funcionalidades e componentes exercitados.

## Histórico de versões

| Versão | Data       | Autor           | Descrição da Alteração                  | Revisor   |
|--------|-----------|----------------|----------------------------------------|--------------|
| 1.0    | 15/04/2026 | [Ana Luiza](https://github.com/luabrantess)  | Adição do planejamento da sprint 1 | |
| 2.0    | 24/06/2026 | Bruno Henryque  | Revisão dos testes 11 a 20 e registro de dificuldades e soluções | |
