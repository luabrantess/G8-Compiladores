# Sprint 5

## Período

* **Duração:** 13 dias
* **Início:** 10 de junho de 2026
* **Prazo final:** 22 de junho de 2026

---

## Objetivo da Sprint

Finalizar o projeto com a implementação da etapa de **Geração de Código**, permitindo a tradução do subconjunto de C suportado para Lua. Além disso, consolidar a documentação, revisar e expandir a suíte de testes e preparar o repositório para a entrega final da disciplina.

---

## Divisão de Tarefas

### Implementação da Geração de Código Lua

**Responsáveis:** [Tulio](https://github.com/TulioCeleri) e [Pedro](https://github.com/G0ndim)

#### Atividades

* Criação dos módulos `codegen.h` e `codegen.c`;
* Integração do módulo de geração de código ao processo de compilação;
* Integração da geração de código ao fluxo principal do compilador;
* Tradução de declarações e atribuições para Lua;
* Tradução de expressões e operadores suportados;
* Tradução das estruturas de controle (`if`, `while` e `for`);
* Tradução de funções e instruções `return`;
* Implementação da tradução de arrays considerando a diferença de indexação entre C e Lua;
* Implementação da tradução de chamadas `printf` para funções equivalentes em Lua;
* Validação do código Lua gerado.

---

### Documentação e Testes

**Responsáveis:** [Ana Luiza](https://github.com/luabrantess), [Samara](https://github.com/samarawwleticia) e [Bruno](https://github.com/Bgrangeiro)

#### Atividades

* Atualização do `README.md` com as funcionalidades implementadas;
* Criação da documentação da Sprint 5;
* Atualização da especificação da linguagem suportada pelo compilador;
* Revisão e reorganização dos testes existentes;
* Separação dos testes válidos e inválidos;
* Desenvolvimento de novos testes para validação da geração de código Lua;
* Criação de uma tabela de cobertura dos testes;
* Atualização do relatório de desenvolvimento do projeto;
* Revisão e atualização da documentação publicada no GitHub Pages/MkDocs;
* Validação final da documentação para entrega.

---

## Entregáveis da Sprint

* Módulo de geração de código Lua integrado ao compilador;
* Tradução funcional do subconjunto de C para Lua;
* Suporte à tradução de variáveis, expressões, estruturas de controle, funções, arrays e `printf`;
* Testes reorganizados e ampliados;
* Casos de teste específicos para validação da saída Lua;
* Documentação técnica atualizada;
* GitHub Pages/MkDocs revisado e pronto para apresentação;
* Projeto finalizado e preparado para entrega.

---

## Histórico de Versões

| Versão | Data       | Autor                                       | Descrição da Alteração                          | Revisor |
| ------ | ---------- | ------------------------------------------- | ----------------------------------------------- | ------- |
| 1.0    | 22/06/2026 | [Ana Luiza](https://github.com/luabrantess) | Adição do planejamento e resultados da Sprint 5 |         |
