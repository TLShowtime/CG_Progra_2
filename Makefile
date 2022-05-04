OBJECTS=raytracer.o
OUTPUT=raytracer
CC=cc

$(OUTPUT): $(OUTPUT).c
	$(CC) -o $(OUTPUT) $(OUTPUT).c $(LDLIBS)


SRCS = $(shell find  -name '*.c')

all:
	cc  -o $(OUTPUT) $(SRCS)  `pkg-config --cflags --libs MagickWand`

run:
	./$(OUTPUT)


clean:
	rm -f *.o
	rm -f raytracer