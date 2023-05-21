CPP1 = klient
CPP2 = serwer
HED1 = kolLib

.PHONY: all clean

all: $(CPP1) $(CPP2)

$(CPP1): $(CPP1).cpp $(HED1).h
	g++ $(CPP1).cpp -o $(CPP1)

$(CPP2): $(CPP2).cpp $(HED1).h
	g++ $(CPP2).cpp -o $(CPP2)

run: all
	xterm -hold -title $(CPP1) -e ./$(CPP1) &
	xterm -hold -title $(CPP1) -e ./$(CPP1) &
	xterm -hold -title $(CPP2) -e ./$(CPP2) &