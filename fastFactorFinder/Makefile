IDIR = .
CC=g++
CFLAGS=-I$(IDIR) -std=c++11 -Wall -Ofast

ODIR=obj
LDIR = .

LIBS=-lboost_thread -lstdc++

_DEPS = 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = fastFactorFinder.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: fastFactorFinder

$(ODIR):
	mkdir -p $(ODIR)

$(ODIR)/%.o: %.cpp $(ODIR) $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

fastFactorFinder: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -rf $(ODIR)
	rm fastFactorFinder

.PHONY: all

