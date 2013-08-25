CFLAGS= -Wall -std=gnu99
LFLAGS= -lreadline
DEBUG_FLAGS= -g
SRCDIR= src
BUILD_DIR= build
SRCFILES= $(wildcard $(SRCDIR)/*.c)
OBJFILES= $(SRCFILES:$(SRCDIR)/%.c=$(BUILD_DIR)/%.o)
CC=gcc
NAME= clockme

$(BUILD_DIR)/%.o : $(SRCDIR)/%.c
	@echo "[CC] Building $< ..."
	@$(CC) $(CFLAGS) -c -o $@ $<

$(NAME): $(OBJFILES)
	@echo "[LINK] Linking $@ ..."
	@$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(OBJFILES)
	@echo "[DEBUG] Building binary with debug symbols..."
	$(CC) $(CFLAGS) -o $(NAME) $^ $(LFLAGS)

.PHONY: clean

clean:
	@echo "[CLEAN] Removing object files and binary..."
	@rm -f ${OBJFILES} clockme
