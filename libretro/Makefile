
TARGET_NAME := fixgb
DEBUG = 0
AUDIO_FLOAT = 0

ifeq ($(platform),)
	platform = unix
	ifeq ($(shell uname -a),)
		platform = win
	else ifneq ($(findstring MINGW,$(shell uname -a)),)
		platform = win
	else ifneq ($(findstring Darwin,$(shell uname -a)),)
		platform = osx
	else ifneq ($(findstring win,$(shell uname -a)),)
		platform = win
	endif
endif

# system platform
system_platform = unix
ifeq ($(shell uname -a),)
	EXE_EXT = .exe
	system_platform = win
else ifneq ($(findstring Darwin,$(shell uname -a)),)
	system_platform = osx
else ifneq ($(findstring MINGW,$(shell uname -a)),)
	system_platform = win
endif

version_script = link.T

ifeq ($(ARCHFLAGS),)
ifeq ($(archs),ppc)
   ARCHFLAGS = -arch ppc -arch ppc64
else
   ARCHFLAGS = -arch i386 -arch x86_64
endif
endif

# Unix
ifeq ($(platform), unix)
	TARGET := $(TARGET_NAME)_libretro.so
	fpic := -fPIC
	SHARED := -shared -Wl,-version-script=$(version_script)

# OS X
else ifeq ($(platform), osx)
	TARGET := $(TARGET_NAME)_libretro.dylib
	fpic := -fPIC
	SHARED := -dynamiclib
	OSXVER = `sw_vers -productVersion | cut -d. -f 2`
	OSX_LT_MAVERICKS = `(( $(OSXVER) <= 9)) && echo "YES"`
	#fpic += -mmacosx-version-min=10.1
	ifndef ($(NOUNIVERSAL))
		CFLAGS += $(ARCHFLAGS)
		CXXFLAGS += $(ARCHFLAGS)
		LDFLAGS += $(ARCHFLAGS)
	endif

# iOS
else ifneq (,$(findstring ios,$(platform)))

	TARGET := $(TARGET_NAME)_libretro_ios.dylib
	fpic := -fPIC
	SHARED := -dynamiclib
	ifeq ($(IOSSDK),)
		IOSSDK := $(shell xcodebuild -version -sdk iphoneos Path)
	endif
	CC = cc -arch armv7 -isysroot $(IOSSDK)
	CXX = c++ -arch armv7 -isysroot $(IOSSDK)
ifeq ($(platform),ios9)
	CC += -miphoneos-version-min=8.0
	CXX += -miphoneos-version-min=8.0
	PLATFORM_DEFINES := -miphoneos-version-min=8.0
else
	CC += -miphoneos-version-min=5.0
	CXX += -miphoneos-version-min=5.0
	PLATFORM_DEFINES := -miphoneos-version-min=5.0
endif

# Theos
else ifeq ($(platform), theos_ios)
	DEPLOYMENT_IOSVERSION = 5.0
	TARGET = iphone:latest:$(DEPLOYMENT_IOSVERSION)
	ARCHS = armv7 armv7s
	TARGET_IPHONEOS_DEPLOYMENT_VERSION=$(DEPLOYMENT_IOSVERSION)
	THEOS_BUILD_DIR := objs
	include $(THEOS)/makefiles/common.mk
	LIBRARY_NAME = $(TARGET_NAME)_libretro_ios

# QNX
else ifeq ($(platform), qnx)
	TARGET := $(TARGET_NAME)_libretro_qnx.so
	fpic := -fPIC
	SHARED := -lcpp -lm -shared -Wl,-version-script=$(version_script)
	CC = qcc -Vgcc_ntoarmv7le
	CXX = QCC -Vgcc_ntoarmv7le_cpp
	AR = QCC -Vgcc_ntoarmv7le
	PLATFORM_DEFINES := -D__BLACKBERRY_QNX__ -fexceptions -marm -mcpu=cortex-a9 -mfpu=neon -mfloat-abi=softfp

# PS3
else ifeq ($(platform), ps3)
	TARGET := $(TARGET_NAME)_libretro_$(platform).a
	CC = $(CELL_SDK)/host-win32/ppu/bin/ppu-lv2-gcc.exe
	CXX = $(CELL_SDK)/host-win32/ppu/bin/ppu-lv2-g++.exe
	AR = $(CELL_SDK)/host-win32/ppu/bin/ppu-lv2-ar.exe
	PLATFORM_DEFINES := -D__CELLOS_LV2__
	STATIC_LINKING = 1

# sncps3
else ifeq ($(platform), sncps3)
	TARGET := $(TARGET_NAME)_libretro_ps3.a
	CC = $(CELL_SDK)/host-win32/sn/bin/ps3ppusnc.exe
	CXX = $(CELL_SDK)/host-win32/sn/bin/ps3ppusnc.exe
	AR = $(CELL_SDK)/host-win32/sn/bin/ps3snarl.exe
	PLATFORM_DEFINES := -D__CELLOS_LV2__
	STATIC_LINKING = 1

# Lightweight PS3 Homebrew SDK
else ifeq ($(platform), psl1ght)
	TARGET := $(TARGET_NAME)_libretro_$(platform).a
	CC = $(PS3DEV)/ppu/bin/ppu-gcc$(EXE_EXT)
	CXX = $(PS3DEV)/ppu/bin/ppu-g++$(EXE_EXT)
	AR = $(PS3DEV)/ppu/bin/ppu-ar$(EXE_EXT)
	PLATFORM_DEFINES := -D__CELLOS_LV2__
	STATIC_LINKING = 1

# PSP
else ifeq ($(platform), psp1)
	TARGET := $(TARGET_NAME)_libretro_$(platform).a
	CC = psp-gcc$(EXE_EXT)
	CXX = psp-g++$(EXE_EXT)
	AR = psp-ar$(EXE_EXT)
	PLATFORM_DEFINES := -DPSP -DCC_RESAMPLER -DCC_RESAMPLER_NO_HIGHPASS
	CFLAGS += -G0
	CXXFLAGS += -G0
	STATIC_LINKING = 1

# Vita
else ifeq ($(platform), vita)
	TARGET := $(TARGET_NAME)_libretro_$(platform).a
	CC = arm-vita-eabi-gcc$(EXE_EXT)
	CXX = arm-vita-eabi-g++$(EXE_EXT)
	AR = arm-vita-eabi-ar$(EXE_EXT)
	PLATFORM_DEFINES := -DCC_RESAMPLER -DCC_RESAMPLER_NO_HIGHPASS
	CFLAGS += -DVITA
	CXXFLAGS += -DVITA
	STATIC_LINKING = 1

# CTR(3DS)
else ifeq ($(platform), ctr)
	TARGET := $(TARGET_NAME)_libretro_$(platform).a
	CC = $(DEVKITARM)/bin/arm-none-eabi-gcc$(EXE_EXT)
	CXX = $(DEVKITARM)/bin/arm-none-eabi-g++$(EXE_EXT)
	AR = $(DEVKITARM)/bin/arm-none-eabi-ar$(EXE_EXT)
	PLATFORM_DEFINES := -DARM11 -D_3DS -DCC_RESAMPLER -DCC_RESAMPLER_NO_HIGHPASS
	CFLAGS += -march=armv6k -mtune=mpcore -mfloat-abi=hard
	CFLAGS += -Wall -mword-relocations
	CFLAGS += -fomit-frame-pointer -ffast-math
	CXXFLAGS += $(CFLAGS)
	STATIC_LINKING = 1

# DOS
else ifeq ($(platform), dos)
	TARGET := $(TARGET_NAME)_libretro_$(platform).a
	CC = i686-pc-msdosdjgpp-gcc
	CXX = i686-pc-msdosdjgpp-g++
	AR = i686-pc-msdosdjgpp-ar
	CFLAGS += -march=i386
	CFLAGS += -Wall
	CFLAGS += -fomit-frame-pointer -ffast-math
	CXXFLAGS += $(CFLAGS)
	STATIC_LINKING = 1

# Raspberry Pi 1
else ifeq ($(platform), rpi1)
	TARGET := $(TARGET_NAME)_libretro.so
	fpic := -fPIC
	SHARED := -shared -Wl,-version-script=$(version_script)
	PLATFORM_DEFINES := -DARM
	CFLAGS += -marm -march=armv6j -mfpu=vfp -mfloat-abi=hard
	CFLAGS += -fomit-frame-pointer -ffast-math
	CXXFLAGS += $(CFLAGS)

# Raspberry Pi 2
else ifeq ($(platform), rpi2)
	TARGET := $(TARGET_NAME)_libretro.so
	fpic := -fPIC
	SHARED := -shared -Wl,-version-script=$(version_script)
	PLATFORM_DEFINES := -DARM
	CFLAGS += -marm -mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard
	CFLAGS += -fomit-frame-pointer -ffast-math
	CXXFLAGS += $(CFLAGS)

# Raspberry Pi 3
else ifeq ($(platform), rpi3)
	TARGET := $(TARGET_NAME)_libretro.so
	fpic := -fPIC
	SHARED := -shared -Wl,-version-script=$(version_script)
	PLATFORM_DEFINES := -DARM
	CFLAGS += -marm -mcpu=cortex-a53 -mfpu=neon-fp-armv8 -mfloat-abi=hard
	CFLAGS += -fomit-frame-pointer -ffast-math
	CXXFLAGS += $(CFLAGS)

# Xbox 360
else ifeq ($(platform), xenon)
	TARGET := $(TARGET_NAME)_libretro_xenon360.a
	CC = xenon-gcc$(EXE_EXT)
	CXX = xenon-g++$(EXE_EXT)
	AR = xenon-ar$(EXE_EXT)
	PLATFORM_DEFINES := -D__LIBXENON__
	STATIC_LINKING = 1

# Nintendo Game Cube
else ifeq ($(platform), ngc)
	TARGET := $(TARGET_NAME)_libretro_$(platform).a
	CC = $(DEVKITPPC)/bin/powerpc-eabi-gcc$(EXE_EXT)
	CXX = $(DEVKITPPC)/bin/powerpc-eabi-g++$(EXE_EXT)
	AR = $(DEVKITPPC)/bin/powerpc-eabi-ar$(EXE_EXT)
	PLATFORM_DEFINES += -DGEKKO -DHW_DOL -mrvl -mcpu=750 -meabi -mhard-float
	PLATFORM_DEFINES += -U__INT32_TYPE__ -U __UINT32_TYPE__ -D__INT32_TYPE__=int
	STATIC_LINKING = 1

# Nintendo Wii
else ifeq ($(platform), wii)
	TARGET := $(TARGET_NAME)_libretro_$(platform).a
	CC = $(DEVKITPPC)/bin/powerpc-eabi-gcc$(EXE_EXT)
	CXX = $(DEVKITPPC)/bin/powerpc-eabi-g++$(EXE_EXT)
	AR = $(DEVKITPPC)/bin/powerpc-eabi-ar$(EXE_EXT)
	PLATFORM_DEFINES += -DGEKKO -DHW_RVL -mrvl -mcpu=750 -meabi -mhard-float
	PLATFORM_DEFINES += -U__INT32_TYPE__ -U __UINT32_TYPE__ -D__INT32_TYPE__=int
	STATIC_LINKING = 1

# Nintendo WiiU
else ifeq ($(platform), wiiu)
	TARGET := $(TARGET_NAME)_libretro_$(platform).a
	CC = $(DEVKITPPC)/bin/powerpc-eabi-gcc$(EXE_EXT)
	CXX = $(DEVKITPPC)/bin/powerpc-eabi-g++$(EXE_EXT)
	AR = $(DEVKITPPC)/bin/powerpc-eabi-ar$(EXE_EXT)
	PLATFORM_DEFINES += -DGEKKO -DWIIU -DHW_RVL -mwup -mcpu=750 -meabi -mhard-float
	PLATFORM_DEFINES += -U__INT32_TYPE__ -U __UINT32_TYPE__ -D__INT32_TYPE__=int
	STATIC_LINKING = 1

# ARM
else ifneq (,$(findstring armv,$(platform)))
	TARGET := $(TARGET_NAME)_libretro.so
	fpic := -fPIC
	SHARED := -shared -Wl,-version-script=$(version_script)
	ifneq (,$(findstring cortexa5,$(platform)))
		PLATFORM_DEFINES += -marm -mcpu=cortex-a5
	else ifneq (,$(findstring cortexa8,$(platform)))
		PLATFORM_DEFINES += -marm -mcpu=cortex-a8
	else ifneq (,$(findstring cortexa9,$(platform)))
		PLATFORM_DEFINES += -marm -mcpu=cortex-a9
	else ifneq (,$(findstring cortexa15a7,$(platform)))
		PLATFORM_DEFINES += -marm -mcpu=cortex-a15.cortex-a7
	else
		PLATFORM_DEFINES += -marm
	endif
	ifneq (,$(findstring softfloat,$(platform)))
		PLATFORM_DEFINES += -mfloat-abi=softfp
	else ifneq (,$(findstring hardfloat,$(platform)))
		PLATFORM_DEFINES += -mfloat-abi=hard
	endif
	PLATFORM_DEFINES += -DARM

# emscripten
else ifeq ($(platform), emscripten)
	TARGET := $(TARGET_NAME)_libretro_$(platform).bc
	STATIC_LINKING = 1

# GCW0
else ifeq ($(platform), gcw0)
	TARGET := $(TARGET_NAME)_libretro.so
	CC = /opt/gcw0-toolchain/usr/bin/mipsel-linux-gcc
	CXX = /opt/gcw0-toolchain/usr/bin/mipsel-linux-g++
	AR = /opt/gcw0-toolchain/usr/bin/mipsel-linux-ar
	fpic := -fPIC
	SHARED := -shared -Wl,-version-script=$(version_script)
	PLATFORM_DEFINES := -DCC_RESAMPLER -DCC_RESAMPLER_NO_HIGHPASS
	CFLAGS += -O3 -fomit-frame-pointer -march=mips32 -mtune=mips32r2 -mhard-float
	CXXFLAGS += $(CFLAGS)

# Windows
else
	TARGET := $(TARGET_NAME)_libretro.dll
	CC = gcc
	CXX = g++
	SHARED := -shared -static-libgcc -static-libstdc++ -Wl,-no-undefined -Wl,-version-script=$(version_script)
   LDFLAGS := -lws2_32

endif

ifeq ($(AUDIO_FLOAT),1)
CFLAGS += -DAUDIO_FLOAT=1
endif


ifeq ($(DEBUG), 1)
	CFLAGS += -O0 -g
	CXXFLAGS += -O0 -g
else
	CFLAGS += -O3
	CXXFLAGS += -O3 -fno-exceptions -fno-rtti -DHAVE_STDINT_H
endif

ifneq ($(SANITIZER),)
    CFLAGS   += -fsanitize=$(SANITIZER)
    CXXFLAGS += -fsanitize=$(SANITIZER)
    LDFLAGS  += -fsanitize=$(SANITIZER)
endif

CXXFLAGS += -std=c++98

CORE_DIR := libgambatte/src

OBJS :=
OBJS += libretro.o
OBJS += ../main.o
OBJS += ../apu.o
OBJS += ../cpu.o
OBJS += ../input.o
OBJS += ../mbc.o
OBJS += ../mem.o
OBJS += ../ppu.o

DEFINES  += -D__LIBRETRO__ $(PLATFORM_DEFINES)
INCFLAGS += -I. -I..


CFLAGS   += $(fpic) $(DEFINES) -Werror=implicit-function-declaration
CXXFLAGS += $(fpic) $(DEFINES) -Werror=implicit-function-declaration

LIBS :=

LDFLAGS += -Wl,--no-undefined

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(INCFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(INCFLAGS)

ifeq ($(platform), theos_ios)
COMMON_FLAGS := -DIOS $(COMMON_DEFINES) $(INCFLAGS) -I$(THEOS_INCLUDE_PATH) -Wno-error
$(LIBRARY_NAME)_CFLAGS += $(CFLAGS) $(COMMON_FLAGS)
$(LIBRARY_NAME)_CXXFLAGS += $(CXXFLAGS) $(COMMON_FLAGS)
${LIBRARY_NAME}_FILES = $(SOURCES_CXX) $(SOURCES_C)
include $(THEOS_MAKE_PATH)/library.mk
else
all: $(TARGET)

ifeq ($(platform), osx)
ifndef ($(NOUNIVERSAL))
   CFLAGS += $(ARCHFLAGS)
   CXXFLAGS += $(ARCHFLAGS)
   LFLAGS += $(ARCHFLAGS)
endif
endif

$(TARGET): $(OBJS)
ifeq ($(STATIC_LINKING), 1)
	$(AR) rcs $@ $(OBJS)
else
	$(CXX) -o $@ $(SHARED) $(OBJS) $(LDFLAGS) $(LIBS)
endif


clean-objs:
	rm -f $(OBJS)

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)

.PHONY: clean clean-objs
endif

install: $(TARGET)
	install -D -m 755 $(TARGET) $(DESTDIR)$(core_installdir)/$(TARGET)

uninstall:
	rm $(DESTDIR)$(core_installdir)/$(TARGET)
