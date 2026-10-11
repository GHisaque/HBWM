C := gcc
CFLAGS := -std=c23 -O2 -Wall -Wextra -I. -Iinclude

EXE := ../music.exe

LDFLAGS := -LC:/msys64/mingw64/lib -L.
LDLIBS := -lmingw32 -lgdi32 -lwinmm

.PHONY: run all clean

RM := del /f /q
RMDIR = rmdir /s /q $(subst /,\\,$(1))
MKDIR = if not exist $(subst /,\\,$(1)) mkdir $(subst /,\\,$(1))

all: $(EXE)

$(EXE): src/main.c ../../HBWM.h ../../Graphics.h ../../Graphics/Window.h
	$(C) $(CFLAGS) -o $@ src/main.c $(LDFLAGS) $(LDLIBS)

run: all
	$(EXE)

clean:
	$(RMDIR) out
	$(MKDIR) out
	$(RM) $(EXE)