# powturbo  (c) Copyright 2013-2019
#  nmake /f makefile.vs
# or
#  nmake "AVX2=1" /f makefile.vs

.SUFFIXES: .c .obj .dllobj

CC = cl /nologo
LD = link /nologo
AR = lib /nologo
CFLAGS = /MD /O2 -I. /W0 
LDFLAGS = 
ARCH =

LIB_LIB = libic.lib
LIB_DLL = ic.dll
LIB_IMP = ic.lib

OBJS = bitpack.obj bitunpack.obj bitutil.obj vp4c.obj vp4d.obj transpose.obj fp.obj trlec.obj trled.obj vint.obj vsimple.obj vsimple.obj
OBJS_SSE = vs\bitpack_sse.obj vs\bitunpack_sse.obj vs\vp4c_sse.obj vs\vp4d_sse.obj vs\transpose_sse.obj
OBJS_AVX2 = vs\bitpack_avx2.obj vs\bitunpack_avx2.obj vs\vp4c_avx2.obj vs\vp4d_avx2.obj vs\transpose_avx2.obj

!if "$(NSIMD)" == "1"
CFLAGS = $(CFLAGS) /DNSIMD
!else
OBJS = $(OBJS) $(OBJS_SSE)
CFLAGS = $(CFLAGS) /D__SSE__ /D__SSE2__ /D__SSE3__ /D__SSSE3__ /D__SSE4_1__ /D__SSE4_2__
!if "$(AVX2)" == "1"
OBJS = $(OBJS) $(OBJS_AVX2)
CFLAGS = $(CFLAGS) /D__AVX2__
ARCH = /arch:AVX2
!endif
!endif

!if "$(CODEC1)" == "1"
CFLAGS = $(CFLAGS) /DCODEC1
!endif

!IF "$(CODEC2)" == "1"
CFLAGS = $(CFLAGS) /DCODEC2
!endif

!IF "($(BLOSC)" == "1"
CFLAGS = $(CFLAGS) /DBLOSC
!endif

DLL_OBJS = $(OBJS:.obj=.dllobj)

all: $(LIB_LIB) icbench.exe icapp.exe

#$(LIB_DLL) $(LIB_IMP) 

#------------
.c.obj:
	$(CC) -c /Fo$@ /O2 $(CFLAGS) $(ARCH) $**

.cc.obj:
	$(CC) -c /Fo$@ /O2 $(CFLAGS) $(ARCH) $**

.c.dllobj:
	$(CC) -c /Fo$@ /O2 $(CFLAGS) $(ARCH) /DLIB_DLL $**

$(LIB_LIB): $(OBJS)
	$(AR) $(ARFLAGS) -out:$@ $(OBJS)

$(LIB_DLL): $(DLL_OBJS)
	$(LD) $(LDFLAGS) -out:$@ -dll -implib:$(LIB_IMP) $(DLL_OBJS)

$(LIB_IMP): $(LIB_DLL)

icbench.exe: icbench.obj vs\getopt.obj plugins.obj eliasfano.obj $(LIB_LIB)
	$(LD) $(LDFLAGS) -out:$@ $**

icapp.exe: icapp.obj vs\getopt.obj plugins.obj eliasfano.obj $(LIB_LIB)
	$(LD) $(LDFLAGS) -out:$@ $**

clean:
	-del *.obj vs\*.obj *.dll *.exe *.exp *.dllobj *.lib *.manifest 2>nul
