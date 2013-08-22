CFLAGS= -Wall -std=gnu99
LFLAGS= -lreadline
OBJFILES=main.o cmd_handlers.o cmd_parse.o logfile.o numfile.o
CC=gcc

%.o : %.c
	@echo "[CC] Building $< ..."
	@$(CC) $(CFLAGS) -c -o $@ $< -g

clockme: $(OBJFILES)
	@echo "[LINK] Linking $@ ..."
	@$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

.PHONY: clean

clean:
	rm -f *.o clockme
