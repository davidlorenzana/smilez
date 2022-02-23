all: smilez_test smilezip smilezcat

CFLAGS = -O2 -Wall -W -ansi -pedantic 
INCLUDES = -I . 

smilez.o: smilez.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o smilez.o smilez.c

smilez_test: smilez_test.c smilez.o
	$(CC) $(CFLAGS) $(INCLUDES) -o smilez_test smilez.o smilez_test.c

smilezip: smilezip.cpp smilez.o
	$(CXX) $(CFLAGS) $(INCLUDES) -o smilezip smilez.o smilezip.cpp

smilezcat: smilezcat.cpp smilez.o
	$(CXX) $(CFLAGS)$(INCLUDES) -o smilezcat smilez.o smilezcat.cpp

clean:
	-rm -f smilezip smilezcat smilez_test smilez.o
