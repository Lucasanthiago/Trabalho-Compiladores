all: scanner_flex scanner_manual

scanner_flex: cminus.l
	flex cminus.l
	gcc -o scanner_flex lex.yy.c

scanner_manual: scanner_manual.c tabela.h
	gcc -Wall -o scanner_manual scanner_manual.c

test: all
	./scanner_flex   valido.cm       > saidas/saida_flex_valido.txt        2>&1 || true
	./scanner_flex   invalido.cm     > saidas/saida_flex_invalido.txt      2>&1 || true
	./scanner_flex   valido_sub.cm   > saidas/saida_flex_sub_valido.txt    2>&1 || true
	./scanner_flex   invalido_sub.cm > saidas/saida_flex_sub_invalido.txt  2>&1 || true
	./scanner_manual valido_sub.cm   > saidas/saida_manual_sub_valido.txt  2>&1 || true
	./scanner_manual invalido_sub.cm > saidas/saida_manual_sub_invalido.txt 2>&1 || true

compara: test
	grep '^[0-9]' saidas/saida_flex_sub_valido.txt   > /tmp/a.txt
	grep '^[0-9]' saidas/saida_manual_sub_valido.txt > /tmp/b.txt
	diff /tmp/a.txt /tmp/b.txt && echo "SAIDAS IDENTICAS token a token"

clean:
	rm -f scanner_flex scanner_manual lex.yy.c

.PHONY: all test compara clean
