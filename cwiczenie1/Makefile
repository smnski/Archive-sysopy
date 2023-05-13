############################################################################
# Autor: Szymon Urbański,                               Krakow, 18.03.2023 #
# Projekt: Procesy                                      Zadanie 1          #
############################################################################

##### KATALOG #####
DIR = $(notdir $(CURDIR))
##### NAZWY PLIKOW #####
NAME1 = cw1a
NAME2 = cw1b
NAME3 = cw1c
NAME4 = cw1d
NAME5 = cw1e
##### PRZYPISYWANIE NAZW #####
EXEC1 = $(NAME1)
OBJS1 = $(NAME1).o
#----------------------
EXEC2 = $(NAME2)
OBJS2 = $(NAME2).o
#----------------------
EXEC3 = $(NAME3)
OBJS3 = $(NAME3).o
#----------------------
EXEC4 = $(NAME4)
OBJS4 = $(NAME4).o
#----------------------
EXEC5 = $(NAME5)
OBJS5 = $(NAME5).o
#----------------------
##### FLAGI I KOMPILATOR #####
COFLAGS = -Wall -O
LDFLAGS = -Wall -O
CO = g++
LD = $(CO)
#############################
%.o: %.c
	$(CO) $(COFLAGS) -c $<
#############################

##### KOMPILACJA WSZYSTKICH PROGRAMOW #####
.PHONY: all
all: $(EXEC1) $(EXEC2) $(EXEC3) $(EXEC4) $(EXEC5)

##### URUCHAMIANIE POSZCZEGOLNYCH PROGRAMOW #####
.PHONY: run1a, run1b, run1c, run1d, run1e

run1a: $(EXEC1)
	./$(EXEC1)

run1b: $(EXEC2)
	./$(EXEC2)

run1c: $(EXEC3)
	./$(EXEC3)

run1d: $(EXEC4)
	./$(EXEC4)

run1e: $(EXEC5)
	./$(EXEC5)
#############################
$(EXEC1): $(OBJS1)
	 $(LD) -o $@ $(LDFLAGS) $^
#############################
$(EXEC2): $(OBJS2)
	 $(LD) -o $@ $(LDFLAGS) $^
#############################
$(EXEC3): $(OBJS3)
	 $(LD) -o $@ $(LDFLAGS) $^
#############################
$(EXEC4): $(OBJS4)
	 $(LD) -o $@ $(LDFLAGS) $^
#############################
$(EXEC5): $(OBJS5)
	 $(LD) -o $@ $(LDFLAGS) $^
#############################
.PHONY: clean tar
##### PORZADKI #####
EXECS = $(EXEC1) $(EXEC2) $(EXEC3) $(EXEC4) $(EXEC5)
clean:                                                     
	rm -f *.o  *~ $(EXECS)
#=========================================================================
##### ARCHIWIZACJA I KOMPRESJA #####
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
#=========================================================================
