CC := gcc
SRCDIR := .
OBJDIR := objdir
CFLAGS := -std=c11

TARGET := $(SRCDIR)/assembler
SOURCES := $(wildcard $(SRCDIR)/*.c )
OBJECTS := $(patsubst $(SRCDIR)/%.c,$(SRCDIR)/$(OBJDIR)/%.o,$(SOURCES))

.PHONY: all debug pedantic release

all: debug

debug: CFLAGS += -g -Og -Wall
debug: $(TARGET)

pedantic: CFLAGS += -Wextra -pedantic
pedantic: debug

release: $(CFLAGS) += -O3


$(TARGET): $(OBJECTS)
	@echo
	@echo " Linking..."
	$(CC) $(CFLAGS) -o $@ $^

$(SRCDIR)/$(OBJDIR)/%.o: %.c
	@echo
	@echo " Building objects..."
	@mkdir -p $(SRCDIR)/$(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $^


.PHONY: clean clobber cleanobj cleanexe
clean: cleanobj
clobber: cleanobj cleanexe

cleanobj:
	@echo
	@echo " Cleaning objects..."
	rm -frv *.o $(OBJDIR)

cleanexe:
	@echo
	@echo " Cleaning executables..."
	rm -frv *.exe $(TARGET)
