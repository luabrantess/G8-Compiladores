# Compilador de C para Lua

## 1. Descrição

Este projeto tem como objetivo o desenvolvimento de um compilador capaz de traduzir código-fonte escrito na linguagem C para a linguagem Lua.

A proposta contempla a implementação das etapas clássicas de compilação, desde a leitura do código até a geração do código alvo. Atualmente, o sistema já implementa o fluxo completo de compilação:

- Análise Léxica  
- Análise Sintática  
- Geração da Árvore Sintática Abstrata (AST)  
- Tabela de Símbolos  
- Análise Semântica (verificação de tipos)  
- Tradução final para Lua  

Com essas etapas, o compilador valida o código C de acordo com regras gramaticais e semânticas previamente definidas e, ao final, produz o código Lua equivalente.

---

## 2. Arquitetura do Compilador

O projeto utiliza ferramentas consolidadas no desenvolvimento de compiladores, organizadas da seguinte forma:

### 2.1 Analisador Léxico (`lexer.l`)

Desenvolvido com Flex, é responsável por ler o código-fonte e convertê-lo em tokens, como palavras-chave, identificadores e operadores.

### 2.2 Analisador Sintático (`parser.y`)

Desenvolvido com Bison, define a gramática da linguagem C, validando a estrutura lógica das construções, como:

- Estruturas condicionais  
- Estruturas de repetição  
- Atribuições  

Durante o reconhecimento das produções, o parser também aciona a construção da Árvore Sintática Abstrata (AST).

### 2.3 Árvore Sintática Abstrata (AST)

A AST representa a estrutura hierárquica do programa de forma independente da sintaxe concreta, servindo de base para as etapas seguintes. Cada nó descreve uma construção da linguagem (declarações, expressões, comandos), permitindo percorrer e manipular o programa de maneira organizada durante a análise semântica e a geração de código.

### 2.4 Tabela de Símbolos

A tabela de símbolos registra os identificadores declarados no programa (variáveis, funções e seus tipos), controlando escopos e permitindo a consulta de informações necessárias para a verificação semântica, como a detecção de variáveis não declaradas ou redeclaradas.

### 2.5 Análise Semântica

Percorrendo a AST e consultando a tabela de símbolos, a análise semântica verifica a coerência do programa além da estrutura sintática. Entre as verificações realizadas estão:

- Verificação de tipos em expressões e atribuições  
- Uso de variáveis previamente declaradas  
- Compatibilidade entre tipos em operações  

### 2.6 Geração de Código Lua

Após a validação semântica, o compilador percorre a AST e gera o código equivalente em Lua, traduzindo as construções da linguagem C (declarações, expressões, estruturas de controle e atribuições) para a sintaxe correspondente em Lua.

---

## 3. Como Rodar os Testes

### 3.1 Pré-requisitos
Antes de iniciar, certifique-se de ter as ferramentas de compilação instaladas em sua máquina. No ambiente Linux (Ubuntu/Debian), você pode instalá-las com o seguinte comando:

```bash
sudo apt install gcc flex bison make
```

### 3.2 Compilação do Projeto
1. Navegue até a pasta `src` do projeto:
   ```bash
   cd src
   ```
2. Execute o comando `make` para compilar o gerador de código e os analisadores:
   ```bash
   make
   ```
   *Isso gerará o executável chamado `./compilador` dentro da pasta `src`.*

### 3.3 Executando um Caso de Teste
Como os testes estão divididos entre casos estruturalmente corretos (**válidos**) e incorretos (**inválidos**), utilize os caminhos correspondentes:

#### Cenário A: Testar um arquivo C válido (Gera código Lua)
Para rodar um teste que não possui erros sintáticos ou semânticos:
```bash
./compilador < ../testes/validos/teste1.c
```
**O que esperar do resultado:**
* O terminal exibirá as mensagens de sucesso da análise sintática, a Árvore Sintática Abstrata (AST) e a Tabela de Símbolos.
* Um arquivo chamado `saida.lua` será gerado automaticamente dentro da pasta `src` com o código correspondente traduzido.

#### Cenário B: Testar um arquivo C inválido (Aponta erros)
Para rodar um teste projetado para falhar (erro léxico, sintático ou semântico):
```bash
./compilador < ../testes/invalidos/teste3.c
```
**O que esperar do resultado:**
* O terminal identificará o motivo exato do travamento da análise (ex: `Erro sintatico`).
* Nenhum código Lua válido será gerado para o arquivo final.

### 3.4 Limpar Arquivos Gerados
Para limpar os arquivos temporários gerados pelo Flex/Bison e redefinir a pasta para uma nova compilação, execute:
```bash
make clean
```

---

## 4. Equipe

<table>
  <tr>
    <td align="center">
      <a href="https://github.com/Bgrangeiro">
        <img src="https://github.com/Bgrangeiro.png" width="100px;" alt="Bgrangeiro"/>
        <br />
        <sub><b>Bgrangeiro</b></sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/luabrantess">
        <img src="https://github.com/luabrantess.png" width="100px;" alt="luabrantess"/>
        <br />
        <sub><b>luabrantess</b></sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/G0ndim">
        <img src="https://github.com/G0ndim.png" width="100px;" alt="G0ndim"/>
        <br />
        <sub><b>G0ndim</b></sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/TulioCeleri">
        <img src="https://github.com/TulioCeleri.png" width="100px;" alt="TulioCeleri"/>
        <br />
        <sub><b>TulioCeleri</b></sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/samarawwleticia">
        <img src="https://github.com/samarawwleticia.png" width="100px;" alt="samarawwleticia"/>
        <br />
        <sub><b>samarawwleticia</b></sub>
      </a>
    </td>
  </tr>
</table>