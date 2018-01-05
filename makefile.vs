# powturbo  (c) Copyright 2013-2018
# nmake "NCODEC1=1" "NCODEC2=1" /f makefile.vs
# or
# nmake "AVX2=1" /f makefile.vs

.SUFFIXES: .c .obj .dllobj

CC = cl /nologo
LD = link /nologo
AR = lib /nologo
CFLAGS = /MD /O2 -I. 
LDFLAGS = 

LIB_LIB = libic.lib
LIB_DLL = ic.dll
LIB_IMP = ic.lib

OBJS = bitpack.obj bitunpack.obj vp4c.obj vp4d.obj transpose.obj bitutil.obj fp.obj vsimple.obj vint.obj

!if "$(AVX2)" == "1"
OBJS = $(OBJS) bitpack_avx2.obj bitunpack_avx2.obj transpose_avx2.obj vp4c_avx2.obj vp4d_avx2.obj 
DEFS = $(DEFS) /D__AVX2__
!endif

!if "$(NSIMD)" == "1"
DEFS = $(DEFS) /DNSIMD
NCODEC1=1
NCODEC2=1
!else
OBJS = $(OBJS) transpose_sse.obj bitpack_sse.obj bitunpack_sse.obj vp4c_sse.obj vp4d_sse.obj
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

#------------
vp4c.obj: vp4c.c
	$(CC) /O2 $(CFLAGS) -c vp4c.c /Fovp4c.obj

vp4c_sse.obj: vp4c.c
	$(CC) /O2 $(CFLAGS) /DSSE2_ON /D__SSSE3__ /arch:SSSE3 /D__SSE2__ /arch:SSE2 /c vp4c.c /Fovp4c_sse.obj

vp4c_avx2.obj: vp4c.c
	$(CC) /O2 $(CFLAGS) /DAVX2_ON /D__AVX2__ /arch:avx2 /c vp4c.c /Fovp4c_avx2.obj
#------------
vp4d.obj: vp4d.c
	$(CC) /O2 $(CFLAGS) -c vp4d.c /Fovp4d.obj

vp4d_sse.obj: vp4d.c
	$(CC) /O2 $(CFLAGS) /DSSE2_ON /D__SSSE3__ /arch:SSSE3 /D__SSE2__ /arch:SSE2 /c vp4d.c /Fovp4d_sse.obj

vp4d_avx2.obj: vp4d.c
	$(CC) /O2 $(CFLAGS) /DAVX2_ON /D__AVX2__ /arch:avx2 /c vp4d.c /Fovp4d_avx2.obj
#------------
bitpack.obj: bitpack.c
	$(CC)  $(CFLAGS) -c bitpack.c /Fobitpack.obj

bitpack_sse.obj: bitpack.c
	$(CC) /O2 $(CFLAGS) /DSSE2_ON /D__SSE2__ /arch:SSE2 /c bitpack.c /Fobitpack_sse.obj

bitpack_avx2.obj: bitpack.c
	$(CC) /O2 $(CFLAGS) /DAVX2_ON /D__AVX2__ /arch:avx2 /c bitpack.c /Fobitpack_avx2.obj

#------------
bitunpack.obj: bitunpack.c
	$(CC) /O2 $(CFLAGS) -c bitunpack.c /Fobitunpack.obj

bitunpack_sse.obj: bitunpack.c
	$(CC) /O2 $(CFLAGS) /DSSE2_ON /D__SSSE3__ /arch:SSSE3 /D__SSE2__ /arch:SSE2 /c bitunpack.c /Fobitunpack_sse.obj

bitunpack_avx2.obj: bitunpack.c
	$(CC) /O2 $(CFLAGS) /DAVX2_ON /D__AVX2__ /arch:avx2 /c bitunpack.c /Fobitunpack_avx2.obj
	
transpose.obj: transpose.c
	$(CC) /O2 $(CFLAGS) -c transpose.c /Fotranspose.obj

transpose_sse.obj: transpose.c
	$(CC) /O2 $(CFLAGS) /DSSE2_ON /D__SSE2__ /arch:SSE2 /c transpose.c /Fotranspose_sse.obj

transpose_avx2.obj: transpose.c
	$(CC) /O2 $(CFLAGS) /DAVX2_ON /D__AVX2__ /arch:avx2 /c transpose.c /Fotranspose_avx2.obj

.c.obj:
	$(CC) -c /Fo$@ /O2 $(CFLAGS) /arch:SSSE3 /D__SSSE3__ $(DEFS) $**

.cc.obj:
	$(CC) -c /Fo$@ /O2 $(CFLAGS) /arch:SSSE3 /D__SSSE3__ $(DEFS) $**

.c.dllobj:
	$(CC) -c /Fo$@ /O2 $(CFLAGS) $(DEFS) /DLIB_DLL $**

$(LIB_LIB): $(OBJS)
	$(AR) $(ARFLAGS) -out:$@ $(OBJS)

$(LIB_DLL): $(DLL_OBJS)
	$(LD) $(LDFLAGS) -out:$@ -dll -implib:$(LIB_IMP) $(DLL_OBJS)

$(LIB_IMP): $(LIB_DLL)

icbench.exe: icbench.obj vs/getopt.obj plugins.obj eliasfano.obj vsimple.obj $(LIB_LIB)
	$(LD) $(LDFLAGS) -out:$@ $**

clean:
	-del *.dll *.exe *.exp *.obj *.dllobj *.lib *.manifest 2>nul
