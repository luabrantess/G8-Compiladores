# Compilador de C para Lua

## 1. Descrição

Este projeto tem como objetivo o desenvolvimento de um compilador capaz de traduzir código-fonte escrito na linguagem C para a linguagem Lua.

A proposta contempla a implementação das etapas clássicas de compilação, desde a leitura do código até a geração do código alvo. Atualmente, o sistema já implementa:

- Análise Léxica  
- Análise Sintática  

Essas etapas garantem a validação estrutural do código C de acordo com regras gramaticais previamente definidas.

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

### 2.3 Evoluções Planejadas

As próximas etapas do projeto incluem:

- Implementação da Tabela de Símbolos  
- Análise Semântica (verificação de tipos)  
- Geração da Árvore Sintática Abstrata (AST)  
- Geração de código intermediário  
- Tradução final para Lua  

---

## Como Rodar os Testes


Na pasta `src`, execute o comando `make` e em seguida `./compilador < ../testes/nome_do_arquivo_de_teste.c`.

**Obs:** É necessário ter instalado em sua máquina o gcc, flex, bison e make. Todos podem ser instalados com o comando `sudo apt install` caso você esteja no ambiente linux.


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
