# Variables
CC = gcc
CFLAGS = -Wall -Wextra -g
LIBS = -lreadline  # Gestion de libreadline

# Cibles
TARGET = fsh
SRC = src
OBJ = $(SRC)/main.o $(SRC)/commands.o $(SRC)/tokenizer.o $(SRC)/interns.o
INCLUDE = $(SRC)/include

# Compilation principale
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBS)

# Compilation des objets
$(SRC)/%.o: $(SRC)/%.c $(INCLUDE)/%.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@

# Nettoyage
clean:
	rm -f $(OBJ) $(TARGET)

# Dépendances
.PHONY: clean   # Indique que 'clean' n'est pas un fichier.
