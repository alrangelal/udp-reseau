# Projet Réseau
# Client - Serveur UDP
# Alejandro RANGEL
# Evrard NGUEMO
# EI-2I HF
# 16/04/2021
# options de compilation
CC = gcc
CCFLAGS = -Wall
LIBS = 
LIBSDIR = 

# fichiers du projet
SRC1 = udpServeur.c
SRC2 = udpClient.c
OBJ1 = $(SRC1:.c=.o)
OBJ2 = $(SRC2:.c=.o)
EXEC1 = server.out
EXEC2 = client.out

# règle initiale 
all: $(EXEC1) $(EXEC2)


# dépendance des .h
udpClient.o : udpServeur.h
udpServeur.o : udpServeur.h

# règles de compilation
%.o: %.c
	$(CC) $(CCFLAGS) -o $@ -c $<
    
# règles d'édition de liens
$(EXEC1): $(OBJ1)
	$(CC) -o $@ $^ $(LIBS) $(LIBSDIR)
$(EXEC2): $(OBJ2)
	$(CC) -o $@ $^ $(LIBS) $(LIBSDIR)

# règles supplémentaires
clean:
	rm -f *.o
mproper:
	rm -f $(EXEC1) $(EXEC2)*.o


