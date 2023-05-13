############################################################################
# Autor: Szymon Urbański,                               Krakow, 3.04.2023 #
# Projekt: Sygnaly                                                        #
############################################################################

##### KATALOG #####
DIR = $(notdir $(CURDIR))
##### NAZWY PLIKOW #####
NAME1 = cw3a
NAME2 = cw3b
NAME3 = cw3c
NAME4 = ChildGen
##### PRZYPISYWANIE NAZW #####
EXEC1 = $(NAME1).x
#----------------------
EXEC2 = $(NAME2).x
#----------------------
EXEC3 = $(NAME3).x
#----------------------
EXEC4 = $(NAME4).x
#----------------------
##### FLAGI I KOMPILATOR #####
COFLAGS = -Wall -O
LDFLAGS = -Wall -O
CO = g++
LD = $(CO)
#############################

##### KOMPILACJA WSZYSTKICH PROGRAMOW BEZ ICH URUCHAMIANIA #####
.PHONY: all
all: $(EXEC1) $(EXEC2) $(EXEC3) $(EXEC4)

##### URUCHAMIANIE POSZCZEGOLNYCH PROGRAMOW #####
.PHONY: run3a, run3b, run3c, rungen

run3a: $(EXEC1)

run3b: $(EXEC2)

run3c: $(EXEC3)

rungen: $(EXEC4)
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
$(EXEC4): $(NAME4).cpp
	 $(LD) -o $@ $(LDFLAGS) $^
#############################

##### PORZADKI #####
.PHONY: clean tar
EXECS = $(EXEC1) $(EXEC2) $(EXEC3) $(EXEC4)

clean:
	rm -f *.o *~ *.x $(EXECS)

##### ARCHIWIZACJA I KOMPRESJA #####
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz $(DIR))