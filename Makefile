# Variables
CC = gcc
CFLAGS = -Wall -Wextra -g
LIBS = -lreadline  #Gestion de libreadline

# Cibles
TARGET = fsh
SRC = src
OBJ = $(SRC)/main.o $(SRC)/commands.o
INCLUDE = $(SRC)/include

# Compilation principale
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBS)

# Compilation des '.c'
$(SRC)/main.o: $(SRC)/main.c $(INCLUDE)/commands.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $(SRC)/main.c -o $(SRC)/main.o

$(SRC)/commands.o: $(SRC)/commands.c $(INCLUDE)/commands.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $(SRC)/commands.c -o $(SRC)/commands.o

# Nettoyage
clean:
	rm -f $(OBJ) $(TARGET)

# Dépendances
.PHONY: clean   # Indique que 'clean' n'est pas un fichier.
