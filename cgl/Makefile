INCLUDE = -I../ -I../numerical/

OUTPUT  = ../bin/cgl/

cgl     : makedirs main.o
	cd $(OUTPUT); g++ -o cgl main.o

makedirs: ;
	-mkdir ../bin
	-mkdir ../bin/cgl

main.o  : main.cpp
	g++ -o $(OUTPUT)main.o -c $(INCLUDE) main.cpp

clean   : 
	rm -r -R $(OUTPUT)
