CC = gcc
CFLAGS = -O2
LDFLAGS = `sdl-config --cflags --libs` -lSDLmain -lSDL -std=c89 -DEMBEDDED
OUTPUT = chip8.elf

SOURCES = chip8.c
OBJS = ${SOURCES:.c=.o}

${OUTPUT}:${OBJS}
	${CC} -o ${OUTPUT} ${SOURCES} ${CFLAGS} ${LDFLAGS} ${DEFINES} 
	
clean:
	rm *.o
	rm ${OUTPUT}
