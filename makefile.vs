# powturbo  (c) Copyright 2013-2019
# nmake /f makefile.vs

.SUFFIXES: .c .obj .dllobj

CC = cl /nologo
LD = link /nologo
AR = lib /nologo
CFLAGS = /MD /O2 -I. /W0
LDFLAGS = 
ARCH = /arch:AVX
AVX2 = 1

LIB_LIB = libic.lib
LIB_DLL = ic.dll
LIB_IMP = ic.lib

OBJS = lib\bic.obj lib\bitpack.obj lib\bitunpack.obj lib\bitutil.obj lib\eliasfano.obj lib\fp.obj lib\iccodec.obj lib\transpose.obj lib\transpose_.obj lib\trlec.obj lib\trled.obj lib\v8.obj lib\v8pack.obj lib\vbit.obj lib\vint.obj lib\vp4c.obj lib\vp4d.obj lib\vsimple.obj 
OBJS_AVX2 = vs\bitpack_avx2.obj vs\bitunpack_avx2.obj vs\bitutil_avx2.obj vs\transpose_avx2.obj vs\vp4c_avx2.obj vs\vp4d_avx2.obj

!if "$(NSIMD)" == "1"
CFLAGS = $(CFLAGS) /DNSIMD
!else
OBJS = $(OBJS)
CFLAGS = $(CFLAGS) /D__SSE__ /D__SSE2__ /D__SSE3__ /D__SSSE3__ /D__SSE4_1__ /D__SSE4_2__
!if "$(AVX2)" == "1"
OBJS = $(OBJS) $(OBJS_AVX2)
# ARCH = /arch:AVX2
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

all: $(LIB_LIB) icapp.exe

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

icapp.exe: lib\icapp.obj vs\getopt.obj $(LIB_LIB)
	$(LD) $(LDFLAGS) -out:$@ $**

clean:
	-del *.obj vs\*.obj lib\*.obj *.dll *.exe *.exp *.dllobj *.lib *.manifest 2>nul
