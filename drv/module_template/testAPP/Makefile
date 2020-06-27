
CROSS_COMPILE := arm-linux-gnueabihf-
CC := $(CROSS_COMPILE)gcc
CFLAG := 

SRC := $(wildcard *.c)
OBJ := $(patsubst %.c,%.o,$(SRC))
TARGET := led_test.out

$(TARGET):$(OBJ)
	$(CC) $(OBJ) $(CFLAG) -o $@
	cp $@ /home/think/nfs

%.o:%.c
	$(CC) $< -c $(CFLAG) -o $@


.PHONEY:clean
clean:
	rm -rf *.o *.out