CC = gcc
RESC = windres
CFLAGS = -O2
TARGET = cbrt.exe
SRCS = cbrt.c
LIBS = -ladvapi32
RESSRC = resources.rc
RESOBJ = resources.o

$(TARGET): $(SRCS) $(RESOBJ)
	$(CC) $(CFLAGS) $(SRCS) $(RESOBJ) $(LIBS) -o $(TARGET)

$(RESOBJ): $(RESSRC)
	$(RESC) $(RESSRC) -o $(RESOBJ)

clean:
	del *.exe *.o