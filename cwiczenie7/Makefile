############################################################################
# Autor: Szymon Urbański,                              Krakow, 13.05.2023 #
# Projekt: Pamiec dzielona                                                #
############################################################################

##### KATALOG #####
DIR = $(notdir $(CURDIR))
##### NAZWY PLIKOW #####
NAME1 = main
NAME2 = konsument
NAME3 = producent
##### PRZYPISYWANIE NAZW #####
EXEC1 = $(NAME1).x
#----------------------
EXEC2 = $(NAME2).x
#----------------------
EXEC3 = $(NAME3).x
#----------------------
##### FLAGI I KOMPILATOR #####
COFLAGS = -Wall -O
LDFLAGS = -Wall -O
CO = g++
LD = $(CO)
#############################

##### KOMPILACJA WSZYSTKICH PROGRAMOW BEZ ICH URUCHAMIANIA #####
.PHONY: all
all: $(EXEC1) $(EXEC2) $(EXEC3)

#############################
$(EXEC1): $(NAME1).cpp
	 $(LD) -o $@ $(LDFLAGS) $^
#############################
$(EXEC2): $(NAME2).cpp
	 $(LD) -o $@ $(LDFLAGS) $^
#############################
$(EXEC3): $(NAME3).cpp
	 $(LD) -o $@ $(LDFLAGS) $^
#############################

##### URUCHOMIENIE GLOWNEGO PROGRAMU PO SKOMPILOWANIU #####
.PHONY: run, compare

compare: $(W) $(Z)
	@diff -s $(W) $(Z)

run: $(EXEC1)
	./$(EXEC1) $(W) $(Z)

##### PORZADKI #####
.PHONY: clean tar
EXECS = $(EXEC1) $(EXEC2) $(EXEC3)

clean:
	rm -f *.o *~ *.x $(EXECS)

##### ARCHIWIZACJA I KOMPRESJA #####
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz $(DIR))
