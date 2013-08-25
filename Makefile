CFLAGS= -Wall -std=gnu99 -g
LFLAGS= -lreadline
SRCDIR= src
BUILD_DIR= build
SRCFILES= $(wildcard $(SRCDIR)/*.c)
OBJFILES= $(SRCFILES:$(SRCDIR)/%.c=$(BUILD_DIR)/%.o)
CC=gcc

$(BUILD_DIR)/%.o : $(SRCDIR)/%.c
	@echo "[CC] Building $< ..."
	@$(CC) $(CFLAGS) -c -o $@ $< -g

clockme: $(OBJFILES)
	@echo "[LINK] Linking $@ ..."
	@$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

.PHONY: clean

clean:
	@echo "[CLEAN] Removing object files and binary..."
	@rm -f ${OBJFILES} clockme
