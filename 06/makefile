CC := gcc
SRCDIR := .
OBJDIR := objdir
CFLAGS := -std=c11

TARGET := $(SRCDIR)/assembler
SOURCES := $(wildcard $(SRCDIR)/*.c )
OBJECTS := $(patsubst $(SRCDIR)/%.c,$(SRCDIR)/$(OBJDIR)/%.o,$(SOURCES))

.PHONY: all debug pedantic release

all: debug

debug: CFLAGS += -g -Wall
debug: $(TARGET)

Og: CFLAGS += -Og
Og: debug

pedantic: CFLAGS += -Wextra -pedantic -Werror
pedantic: debug

release: CFLAGS += -O3
release: $(TARGET)


$(TARGET): $(OBJECTS) $(@:=.d)
	@echo
	@echo " Linking..."
	$(CC) $(CFLAGS) -o $@ $^

$(SRCDIR)/$(OBJDIR)/%.o: %.c
	@echo
	@echo " Building objects..."
	@mkdir -p $(SRCDIR)/$(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

%.d:
	@echo
	@echo " Generating dependencies..."
	$(CC) -MM -MT "$(SRCDIR)/$(OBJDIR)/$(@:.d=.o) $(SRCDIR)/$@" -MF $@ $(@:.d=.c)


ifneq ($(MAKECMDGOALS),clean)
-include $(SOURCES:.c=.d)
endif

.PHONY: clean clobber cleanobj cleanexe cleanmake cleanstackdump
clean: cleanobj cleanmake
clobber: clean cleanexe cleanstackdump

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
	rm -frv *.d

cleanstackdump:
	@echo
	@echo " Cleaning stack dumps..."
	rm -frv *.stackdump
