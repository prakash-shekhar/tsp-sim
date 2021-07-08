LIBS=-lsfml-graphics -lsfml-window -lsfml-system

all:
	@echo "** Building the simulation"
	g++ -g -c "main.cpp" -o main.o
	g++ -g -c "Simulation.cpp" -o Simulation.o
	g++ -o tsp-sim main.o Simulation.o $(LIBS)

install:
	sudo install -Dt /usr/local/bin tsp-sim

debug:
	gdb tsp-sim