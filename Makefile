CC = gcc
CFLAGS = -O0 -g
LDFLAGS = `sdl-config --cflags --libs` -lSDLmain -lSDL -std=c89
OUTPUT = chip8.elf

SOURCES = chip8.c
OBJS = ${SOURCES:.c=.o}

${OUTPUT}:${OBJS}
	${CC} -o ${OUTPUT} ${SOURCES} ${CFLAGS} ${LDFLAGS} ${DEFINES} 
	
clean:
	rm *.o
	rm ${OUTPUT}
