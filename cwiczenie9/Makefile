############################################################################
# Autor: Szymon Urbański,                               Krakow, 3.04.2023 #
# Projekt: Sygnaly                                                        #
############################################################################

##### KATALOG #####
DIR = $(notdir $(CURDIR))
##### NAZWY PLIKOW #####
NAME1 = main
##### PRZYPISYWANIE NAZW #####
EXEC1 = $(NAME1).x
#-----------------------
##### FLAGI I KOMPILATOR #####
COFLAGS = -Wall -O
LDFLAGS = -Wall -O
CO = g++
LD = $(CO)
#############################

##### KOMPILACJA WSZYSTKICH PROGRAMOW BEZ ICH URUCHAMIANIA #####
.PHONY: all
all: $(EXEC1)

##### URUCHAMIANIE POSZCZEGOLNYCH PROGRAMOW #####
.PHONY: run

run: $(EXEC1)
	./$(EXEC1) $(W) $(S)
#############################
$(EXEC1): $(NAME1).cpp
	 $(LD) -o $@ $(LDFLAGS) $^
#############################

##### PORZADKI #####
.PHONY: clean tar
EXECS = $(EXEC1)

clean:
	rm -f *.o *~ *.x $(EXECS)

##### ARCHIWIZACJA I KOMPRESJA #####
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz $(DIR))