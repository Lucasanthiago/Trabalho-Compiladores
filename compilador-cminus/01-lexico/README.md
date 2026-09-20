# Entrega 1 — Analisador Léxico (C-Minus)

Duas trilhas, como pede o enunciado: uma manual sobre um subconjunto de regras
e uma gerada por ferramenta sobre a BNF completa. Ambas compilam e rodam.

## Subconjunto de regras

Regras 18, 20, 22, 23, 24, 25 e 26 — o sub-idioma das expressões. Mais as
auxiliares 19 (`var`), 21 (`relop`) e 27 (`call`), sem as quais os terminais
referenciados pelas regras 20 e 26 ficariam indefinidos.

Resultado: 17 tokens, nenhuma palavra reservada, e três casos de maior
casamento possível (`=` contra `==`, `<` contra `<=`, `>` contra `>=`).

Números da construção: **55** estados no NFA de Thompson, **21** no DFA de
subconjuntos, **19** no DFA mínimo. A derivação completa está nas seções 6, 7
e 8 do relatório.

## Arquivos

| Arquivo | O que é |
|---|---|
| `tabela.h` | tabela δ do DFA mínimo, transcrita da seção 8 |
| `scanner_manual.c` | motor do scanner: maior casamento, linha/coluna, comentários |
| `cminus.l` | especificação Flex da BNF completa das 29 regras |
| `valido_sub.cm`, `invalido_sub.cm` | testes da trilha manual |
| `valido.cm`, `invalido.cm` | testes da BNF completa |
| `saidas/` | saídas reais das seis execuções |

## Construir e rodar

```bash
make            # compila os dois scanners
make test       # roda os dois sobre os arquivos de teste
make compara    # confere se as duas trilhas batem token a token
```

Dependências: `flex`, `gcc`.

## Como o scanner manual funciona

O laço não roda o autômato até o fim da entrada. A cada token ele avança
enquanto houver transição, guardando a última posição em que passou por um
estado final; quando trava, recua para essa posição, emite o token guardado e
recomeça dali. Sem esse recuo, `<=` sairia como `<` seguido de `=`.

Espaços e comentários `/* */` são descartados fora do autômato, no laço.

## Resultado da comparação

Sobre `valido_sub.cm` os dois scanners produzem as mesmas 79 linhas, token a
token, com as mesmas linhas e colunas. `make compara` verifica isso.

Sobre os arquivos da BNF completa as saídas divergem de propósito, em dois
pontos explicados na seção 12.3 do relatório: o scanner manual não conhece
caracteres fora do subconjunto (`;`, `{`, `}`), e trata `12abc` como `NUM`
seguido de `ID`, que é o resultado correto do maior casamento — o `.l` tem uma
regra extra que transforma isso em erro.
