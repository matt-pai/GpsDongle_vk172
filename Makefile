
CC := $(CROSS_COMPILE)gcc
SRCS := $(foreach n, $(wildcard src/*.c), $(n))
OBJS := $(SRCS:.c=.o)

C_FLAGS := -Iinc
LD_FLAGS := -Llib -lnmea -lm

gpsd: $(OBJS)
	$(CC) $(C_FLAGS) -o $@ $(OBJS) $(LD_FLAGS)

%.o: %.c
	$(CC) $(C_FLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) gpsd
