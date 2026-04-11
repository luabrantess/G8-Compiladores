# Nome do executavel final
EXEC = compilador

# Arquivos
LEX = lexer.l
YACC = parser.y

# Arquivos gerados
LEX_C = lex.yy.c
YACC_C = parser.tab.c
YACC_H = parser.tab.h

# Compilador
CC = gcc

# Regras principais
all: $(EXEC)

$(EXEC): $(LEX_C) $(YACC_C)
	$(CC) $(LEX_C) $(YACC_C) -o $(EXEC)

# Gerar parser (Bison)
$(YACC_C) $(YACC_H): $(YACC)
	bison -d $(YACC)

# Gerar lexer (Flex)
$(LEX_C): $(LEX) $(YACC_H)
	flex $(LEX)

# Limpeza
clean:
	rm -f $(EXEC) $(LEX_C) $(YACC_C) $(YACC_H)

# Rodar com entrada padrao
run: $(EXEC)
	./$(EXEC)