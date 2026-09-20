# Compilador C-Minus

Compilador para a linguagem C-Minus, desenvolvido na disciplina de Construção
de Compiladores. O projeto é dividido em quatro etapas, cada uma na sua pasta.

| Etapa | Pasta | Situação |
|---|---|---|
| Analisador léxico | `01-lexico/` | concluída |
| Analisador sintático e semântico | `02-sintatico/` | a fazer |
| Tradução para MIPS | `03-mips/` | a fazer |
| Execução em simulador | `04-simulador/` | a fazer |

## Integrantes

- (preencher)
- (preencher)

## Etapa 1 — Analisador léxico

Duas trilhas independentes sobre a mesma linguagem, comparadas ao final. Uma
manual, sobre um subconjunto de regras de produção, com a conversão
ER → NFA → DFA → DFA mínimo feita à mão. Outra gerada com Flex, cobrindo a BNF
completa das 29 regras.

```bash
cd 01-lexico
make            # compila os dois scanners
make test       # roda os dois sobre os arquivos de teste
make compara    # confere se as duas trilhas batem token a token
```

Dependências: `flex` e `gcc`.

O relatório da etapa, com a derivação completa dos autômatos e os diagramas,
está em `01-lexico/`. Detalhes de implementação no README daquela pasta.
