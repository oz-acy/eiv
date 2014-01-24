# Makefile for EIV

CC=x86_64-w64-mingw32-gcc
CXX=x86_64-w64-mingw32-g++
WRES=x86_64-w64-mingw32-windres

libdir = /usr/local/mw/lib

CXXFLAGS = -c -O2 -DWINVER=0x500 -D_UNICODE -DUNICODE -std=gnu++11 -I/usr/local/mw/include
LDFLAGS = -mwindows -static -L$(libdir)

EIV_OBJ = main.o util.o eiv.o eiv.res.o eivwmh.o
EIV_PLUS_OBJ = main-plus.o util.o eiv2.o eivwmh.o \
	eiv-plus.o savepct.o towallpaper.o eiv-plus.res.o

URANIA_OBJ = $(libdir)/wmain.o
LIBS = -lurania -lpolymnia -lthemis -ljpeg -lpng -lz


####
all: eiv eiv-plus


####
eiv: $(EIV_OBJ)
	$(CXX) $(LDFLAGS) $(EIV_OBJ) $(URANIA_OBJ) $(LIBS) -o $@
	strip $@.exe
#	upx $@.exe

eiv-plus: $(EIV_PLUS_OBJ)
	$(CXX) $(LDFLAGS) $(EIV_PLUS_OBJ) $(URANIA_OBJ) $(LIBS) -o $@
	strip $@.exe
#	upx $@.exe



####
.cpp.o:
	$(CXX)  $(CXXFLAGS) $<


####
eiv.o: eiv.cpp eiv.h
eivwmh.o: eivwmh.cpp eiv.h
eiv2.o: eiv2.cpp eiv.h

eiv-plus.o: eiv.cpp eiv.h
	$(CXX)  $(CXXFLAGS) -DEIV_PLUS -o eiv-plus.o $<


main.o: main.cpp eiv.h painthandler.h

main-plus.o: main.cpp eiv.h painthandler.h
	$(CXX)  $(CXXFLAGS) -DEIV_PLUS -o main-plus.o $<


savepct.o: savepct.cpp eiv.h
toindex.o: toindex.cpp
towallpaper.o: towallpaper.cpp eiv.h
util.o: util.cpp eiv.h
colcnv.o: colcnv.cpp

##painthandler.o: painthandler.cpp painthandler.h eiv.h

####
eiv.res.o : eiv.rc icon1.ico eiv_res.h
	$(WRES) -o $@ eiv.rc

eiv-plus.res.o: eiv-plus.rc icon1.ico eiv_res.h
	$(WRES) -o $@ eiv-plus.rc

####
clean:
	-rm -f *~ *.exe *.o


#eof
