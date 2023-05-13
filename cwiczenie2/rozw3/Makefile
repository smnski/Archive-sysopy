############################################################################
# Autor: Szymon Urbański,                               Krakow, 23.03.2023 #
# Projekt: Procesy                                      Zadanie 2          #
############################################################################

##### KATALOG #####
DIR = $(notdir $(CURDIR))
##### NAZWY PLIKOW #####
NAME1 = rozw
NAME2 = wypisz
##### PRZYPISYWANIE NAZW #####
EXEC1 = $(NAME1)
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

##### KOMPILACJA WSZYSTKICH PROGRAMOW BEZ ICH URUCHAMIANIA #####
.PHONY: all
all: $(EXEC1) $(EXEC2)

##### URUCHAMIANIE POSZCZEGOLNYCH PROGRAMOW #####
.PHONY: run, runPrep

run: $(EXEC1)
	./$(EXEC1) $(nazwa)

runPrep: $(EXEC2)

#############################
$(EXEC1): $(OBJS1)
	 $(LD) -o $@ $(LDFLAGS) $^
#############################
$(EXEC2): $(NAME2).cpp
	 $(LD) -o $@ $(LDFLAGS) $^
#############################

##### PORZADKI #####
.PHONY: clean tar
EXECS = $(EXEC1) $(EXEC2)

clean:
	rm -f *.o *~ *.x $(EXECS)

##### ARCHIWIZACJA I KOMPRESJA #####
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz $(DIR))