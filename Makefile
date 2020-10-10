TARGET = flashmod
OBJS = main.o graphics.o framebuffer.o

CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LIBS = -lpspgu -lpng -lz -lm
LDFLAGS =

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_ICON = ICON0.png
PSP_EBOOT_TITLE = FlashMod Version 1.5

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak