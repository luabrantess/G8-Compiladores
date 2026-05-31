# Sprint 3

## Período

- **Duração:** 15 dias
- **Início:** 29 de abril de 2026
- **Prazo final:** 13 de maio de 2026

---

## Objetivo da Sprint

Implementar a **Árvore Sintática Abstrata (AST)** e a **Tabela de Símbolos**, componentes fundamentais para as próximas etapas do compilador. A AST será responsável por representar a estrutura do programa de forma hierárquica, enquanto a Tabela de Símbolos armazenará informações semânticas sobre identificadores, tipos, funções e escopos.

---

## Divisão de Tarefas

### Criação da AST (Árvore Sintática Abstrata)

**Responsáveis:** [Tulio](https://github.com/TulioCeleri) e [Pedro](https://github.com/G0ndim)

**Prazo interno:** até 06/05/2026

#### Atividades

- Definição da estrutura dos nós da AST em `ast.h`;
- Implementação das funções de criação, manipulação e liberação dos nós em `ast.c`;
- Integração da AST às regras gramaticais do arquivo `parser.y`;

---

### Criação da Tabela de Símbolos

**Responsáveis:** [Ana Luiza](https://github.com/luabrantess), [Samara](https://github.com/samarawwleticia) e [Bruno](https://github.com/Bgrangeiro)

**Prazo interno:** de 06/05/2026 a 13/05/2026

#### Atividades

- Implementação da estrutura da tabela de símbolos (lista encadeada ou tabela hash);
- Armazenamento de informações sobre variáveis, funções, tipos e escopos;
- Desenvolvimento das operações de inserção, busca e gerenciamento de símbolos;

---


## Entregáveis da Sprint

- Implementação completa da AST;
- Integração da AST ao analisador sintático;
- Implementação da Tabela de Símbolos;
- Operações de gerenciamento de símbolos e escopos;

---

## Histórico de Versões

| Versão | Data | Autor | Descrição da Alteração | Revisor |
|---------|---------|---------|---------|---------|
| 1.0 | 31/05/2026 | [Samara Letícia](https://github.com/samarawwleticia) | Adição do planejamento da Sprint 3 | |