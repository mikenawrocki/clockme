CFLAGS= -Wall -std=gnu99 -g
LFLAGS= -lreadline
SRCDIR= src
SRCFILES= $(wildcard $(SRCDIR)/*.c)
OBJFILES= $(patsubst %.c, %.o, $(SRCFILES))
CC=gcc

%.o : %.c
	@echo "[CC] Building $< ..."
	@$(CC) $(CFLAGS) -c -o $@ $< -g

clockme: $(OBJFILES)
	@echo "[LINK] Linking $@ ..."
	@$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

.PHONY: clean

clean:
	rm -f ${OBJFILES} clockme
