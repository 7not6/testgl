target   := test
sources  := src/main.c src/input.c src/matrix.c src/opengl.c src/glext.c
sources  += src/shader-util.c src/shader.c src/sprite.c src/text.c src/util.c 

INCLUDES := -I.
CFLAGS   := -Wall -O2 -g $(INCLUDES)
LDFLAGS  := -static-libgcc
LIBS     := -lm -lGL #-mconsole
packages := sdl2 

# do not edit from here onwards
objects := $(addprefix build/,$(sources:.c=.o))
ifneq ($(packages),)
    LIBS    += $(shell pkg-config --libs-only-l $(packages))
    LDFLAGS += $(shell pkg-config --libs-only-L --libs-only-other $(packages))
    CFLAGS  += $(shell pkg-config --cflags $(packages))
endif

.PHONY: all clean

all: $(target)
clean:
	-rm -rf build
	-rm -f $(target)

$(target): Makefile $(objects)
	$(CC) $(LDFLAGS) -o $@ $(objects) $(LIBS)

build/%.o: %.c Makefile
	-mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -MMD -o $@ $<

-include $(addprefix build/,$(sources:.c=.d))

