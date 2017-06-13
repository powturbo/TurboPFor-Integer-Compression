# nmake "NCODEC1=1" "NCODEC2=1" /f makefile.msc
# or
# nmake "AVX2=1" /f makefile.msc

.SUFFIXES: .c .obj .dllobj

CC = cl /nologo
LD = link /nologo
AR = lib /nologo
CFLAGS = /MD /O2 -I.
LDFLAGS = 

LIB_LIB = libic.lib
LIB_DLL = ic.dll
LIB_IMP = ic.lib

OBJS = bitpack.obj bitutil.obj fp.obj transpose.obj vp4c.obj vp4d.obj vsimple.obj bitunpack.obj vint.obj 

!IF "$(AVX2)" == "1"
OBJS = $(OBJS) transpose_avx2.obj
DEFS = $(DEFS) /D__AVX2__
!endif

!if "$(NSIMD)" == "1"
OBJS = $(OBJS) transpose_sse.obj
DEFS = $(DEFS) /DNSIMD
NCODEC1=1
NCODEC2=1
!else
DEFS = $(DEFS) /D__SSE2__ /D__SSSE3__
CFLAGS = $(CFLAGS) /DUSE_SSE
!endif

!if "$(NCODEC1)" == "1"
DEFS = $(DEFS) /DNCODEC1
!endif

!IF "$(NCODEC2)" == "1"
DEFS = $(DEFS) /DNCODEC2
!endif

!IF "($(BLOSC)" == "1"
DEFS = $(DEFS) /DBLOSC
!endif

DLL_OBJS = $(OBJS:.obj=.dllobj)

all: $(LIB_LIB) icbench.exe 

#$(LIB_DLL) $(LIB_IMP) 

transpose.obj: transpose.c
	$(CC) /O2 $(CFLAGS) -c transpose.c /Fotranspose.obj

transpose_sse.obj: transpose.c
	$(CC) /O2 $(CFLAGS) /DSSE2_ON /D__SSE2__ /arch:SSE2 /c transpose.c /Fotranspose_sse.obj

transpose_avx2.obj: transpose.c
	$(CC) /O2 $(CFLAGS) /DAVX2_ON /D__AVX2__ /arch:avx2 /c transpose.c /Fotranspose_avx2.obj

.c.obj:
	$(CC) -c /Fo$@ /O2 $(CFLAGS) $(DEFS) $**

.cc.obj:
	$(CC) -c /Fo$@ /O2 $(CFLAGS) $(DEFS) $**

.c.dllobj:
	$(CC) -c /Fo$@ /O2 $(CFLAGS) $(DEFS) /DLIB_DLL $**

$(LIB_LIB): $(OBJS)
	$(AR) $(ARFLAGS) -out:$@ $(OBJS)

$(LIB_DLL): $(DLL_OBJS)
	$(LD) $(LDFLAGS) -out:$@ -dll -implib:$(LIB_IMP) $(DLL_OBJS)

$(LIB_IMP): $(LIB_DLL)

icbench.exe: icbench.obj vs/getopt.obj plugins.obj eliasfano.obj vsimple.obj transpose.obj transpose_sse.obj $(LIB_LIB)
	$(LD) $(LDFLAGS) -out:$@ $**

clean:
	-del *.dll *.exe *.exp *.obj *.dllobj *.lib *.manifest 2>nul
