CFLAGS = -Wall
CC = gcc
LIBSRC = main.c common.c df1.c serial.c read_A2.c calc_address.c read_boolean.c read_float.c read_integer.c	select_fnct.c read_socket.c write_AA.c write_integer.c write_float.c write_AB.c write_boolean.c server.c
LIBOBJS := $(patsubst %.c,%.o,$(LIBSRC))
TARGET = df1

all: $(TARGET) clean

$(TARGET): $(LIBOBJS)

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-rm $(LIBOBJS)
