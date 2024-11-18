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

# Compilation des '.c'
$(SRC)/main.o: $(SRC)/main.c $(INCLUDE)/main.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $(SRC)/main.c -o $(SRC)/main.o

$(SRC)/commands.o: $(SRC)/commands.c $(INCLUDE)/commands.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $(SRC)/commands.c -o $(SRC)/commands.o

$(SRC)/tokenizer.o: $(SRC)/tokenizer.c $(INCLUDE)/tokenizer.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $(SRC)/tokenizer.c -o $(SRC)/tokenizer.o

$(SRC)/interns.o: $(SRC)/interns.c $(INCLUDE)/interns.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $(SRC)/interns.c -o $(SRC)/interns.o


# Nettoyage
clean:
	rm -f $(OBJ) $(TARGET)

# Dépendances
.PHONY: clean   # Indique que 'clean' n'est pas un fichier.
