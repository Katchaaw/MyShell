# Variables
CC = gcc
CFLAGS = -Wall -Wextra -g 
LIBS = -lreadline -lm # Gestion de libreadline

# Cibles
TARGET = fsh
SRC = src
OUT = out
OBJ = $(OUT)/main.o $(OUT)/commands.o $(OUT)/tokenizer.o $(OUT)/interns.o $(OUT)/prompt.o $(OUT)/externs.o $(OUT)/redirections.o $(OUT)/for.o
INCLUDE = $(SRC)/include

# Compilation principale
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBS)

# Compilation des objets
$(OUT)/%.o: $(SRC)/%.c $(INCLUDE)/%.h | $(OUT)
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@

# Créer le répertoire 'out'
$(OUT):
	mkdir -p $(OUT)

# Nettoyage
clean:
	rm -f $(OUT)/*.o $(TARGET)
	rmdir $(OUT) 2>/dev/null || true

# Dépendances
.PHONY: clean   # Indique que 'clean' n'est pas un fichier.
