############################################################################
# Autor: Szymon Urbański,                               Krakow, 23.03.2023 #
# Projekt: Procesy                                      Zadanie 2          #
############################################################################

##### KATALOG #####
DIR = $(notdir $(CURDIR))
##### NAZWY PLIKOW #####
NAME1 = test
NAME2 = wypisz
##### PRZYPISYWANIE NAZW #####
EXEC1 = $(NAME1).x
OBJS1 = $(NAME1).o
#----------------------
EXEC2 = $(NAME2).x
#----------------------
##### FLAGI I KOMPILATOR #####
COFLAGS = -Wall -O
LDFLAGS = -Wall -O
CO = g++
LD = $(CO)
#############################

##### KOMPILACJA WSZYSTKICH PROGRAMOW #####
.PHONY: all
all: $(EXEC1) $(EXEC2)

##### URUCHAMIANIE POSZCZEGOLNYCH PROGRAMOW #####
.PHONY: run, runPrep

run: $(EXEC1)
	./$(EXEC1)

runPrep: $(EXEC2)

#############################
$(EXEC1): $(OBJS1)
	 $(LD) -o $@ $(LDFLAGS) $^
#############################
$(EXEC2): $(NAME2).cpp
	 $(LD) -o $@ $(LDFLAGS) $^
#############################
	
.PHONY: all clean tar

clean:
	rm -f *.o *~ *.x $(EXEC)

tar: clean
	(cd ..; tar -cvzf $(NAME).tar.gz $(NAME))