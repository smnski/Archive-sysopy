############################################################################
# Autor: Szymon Urbański,                              Krakow, 21.05.2023 #
# Projekt: Kolejki komunikatow                                            #
############################################################################

##### KATALOG #####
DIR = $(notdir $(CURDIR))

##### NAZWY PLIKOW #####
CPP1 = klient
CPP2 = serwer
HED1 = kolLib

##### FLAGI I KOMPILATOR #####
COFLAGS = -Wall -O
LDFLAGS = -Wall -O
CO = g++
LD = $(CO)
#############################

##### KOMPILACJA WSZYSTKICH PROGRAMOW BEZ ICH URUCHAMIANIA #####
.PHONY: all

all: $(CPP1).x $(CPP2).x

$(CPP1).x: $(CPP1).cpp $(HED1).h
	g++ $(CPP1).cpp -o $(CPP1).x

$(CPP2).x: $(CPP2).cpp $(HED1).h
	g++ $(CPP2).cpp -o $(CPP2).x

##### URUCHOMIENIE GLOWNEGO PROGRAMU #####
run: all
	xterm -hold -title $(CPP2).x -e ./$(CPP2).x &
	xterm -hold -title $(CPP1)1.x -e ./$(CPP1).x &
	xterm -hold -title $(CPP1)2.x -e ./$(CPP1).x &

##### PORZADKI #####
.PHONY: clean tar
EXECS = $(EXEC1) $(EXEC2) $(EXEC3)

clean:
	rm -f *.o *~ *.x $(EXECS)

##### ARCHIWIZACJA I KOMPRESJA #####
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz $(DIR))