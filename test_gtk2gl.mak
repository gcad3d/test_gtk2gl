# make -f test_gtk2gl.mak


SRC = test_gtk2gl.c


#-----------------------------------------
$(info - MAKE_HOST = $(MAKE_HOST))

# get <fn>.o from <fn>.c for sourceList
OBJ = $(patsubst %.c,%.o, $(notdir $(SRC)))

DEBUG = -ggdb -pg


#-----------------------------------------------------
ifeq "$(MAKE_HOST)" "x86_64-pc-linux-gnu"
# Linux-64:
CPFLG = $(DEBUG)\
 -I/usr/include/gtkgl-2.0\
 `pkg-config --cflags gtk+-2.0`

LKFLG = $(DEBUG)\
 -lgtkgl-2.0 -lGL\
 `pkg-config gtk+-2.0 --libs`


#-----------------------------------------------------
else
# MSYS: x86_64-pc-msys
CPFLG = $(DEBUG)\
 -I/c/msys64/mingw64/include/gtkgl-2.0\
 `pkg-config --cflags gtk+-2.0`

LKFLG = $(DEBUG)\
 -L/c/msys64/mingw64/lib\
 -lgtkgl-2.0.dll -lopengl32\
 `pkg-config --libs gtk+-2.0`

endif


#-----------------------------------------------------
build: $(OBJ)
	@echo ================= Link test_gtk2gl ========================
	@echo "SRC=" $(SRC)
	@echo "OBJ=" $(OBJ)
	$(CC) $(OBJ) $(LKFLG) -o test_gtk2gl


#-----------------------------------------------------
clean:
	rm -rf *.o *.obj *.out *.a *.ilk *.pdb *.exp *.lib *.dll *.so *.exe test_gtk2gl


#-----------------------------------------------------
.c.o:
	@echo ================= Compile $@ ========================
	$(CC) -c $(CPFLG) $<


# eof
