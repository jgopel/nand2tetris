CC := gcc
SRCDIR := .
OBJDIR := $(SRCDIR)/.objdir
DEPDIR := $(SRCDIR)/.depdir
EXEDIR := .
CFLAGS := -std=c11

TARGET := $(SRCDIR)/assembler
SOURCES := $(wildcard $(SRCDIR)/*.c )
OBJECTS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))
DEPS :=    $(patsubst $(SRCDIR)/%.c,$(DEPDIR)/%.d,$(SOURCES))

.PHONY: all debug pedantic release

all: debug

debug: CFLAGS += -g -Wall
debug: $(TARGET)

Og: CFLAGS += -Og
Og: debug

pedantic: CFLAGS += -Wextra -pedantic
pedantic: debug

release: CFLAGS += -O3
release: $(TARGET)


$(TARGET): $(OBJECTS)
	@echo
	@echo " Linking..."
	$(CC) $(CFLAGS) -o $(EXEDIR)/$@ $^ -lm

$(OBJDIR)/%.o: %.c %.d
	@echo
	@echo " Building objects..."
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

%.d: %.c
	@echo
	@echo " Generating dependencies..."
	@mkdir -p $(DEPDIR)
	$(CC) -MM -MT "$(OBJDIR)/$(@:.d=.o) $(DEPDIR)/$@" -MF $(DEPDIR)/$@ $(@:.d=.c)


ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif

.PHONY: clean clobber cleanobj cleanexe cleanmake cleanstackdump cleanhack
clean: cleanexe cleanhack
clobber: cleanobj cleanexe cleanmake cleanstackdump cleanhack

cleanobj:
	@echo
	@echo " Cleaning objects..."
	rm -frv *.o $(OBJDIR)

cleanexe:
	@echo
	@echo " Cleaning executables..."
	rm -frv *.exe $(TARGET)

cleanmake:
	@echo
	@echo " Cleaning dependency files..."
	rm -frv *.d $(DEPDIR)

cleanstackdump:
	@echo
	@echo " Cleaning stack dumps..."
	rm -frv *.stackdump

cleanhack:
	@echo
	@echo " Cleaning .hack files..."
	rm -frv *.hack
