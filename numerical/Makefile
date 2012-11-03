INCLUDE = -I../ 

OUTDIR = ../bin/numerical

numerical: makedirs main.o
	cd $(OUTDIR); g++ -o numerical main.o

makedirs: ;
	-mkdir ../bin
	-mkdir $(OUTDIR)

main.o: main.cpp
	g++ -o $(OUTDIR)/main.o -c $(INCLUDE) main.cpp
