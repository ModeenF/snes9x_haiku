## BeOS Generic Makefile v2.5 ##

## Fill in this file to specify the project being created, and the referenced
## makefile-engine will do all of the hard work for you.  This handles both
## Intel and PowerPC builds of the BeOS and Haiku.

## Application Specific Settings ---------------------------------------------

# specify the name of the binary
NAME = ../BeS9x_bin/BeS9x

# specify the type of binary
#	APP:	Application
#	SHARED:	Shared library or add-on
#	STATIC:	Static library archive
#	DRIVER: Kernel Driver
TYPE = APP

# 	if you plan to use localization features 
# 	specify the application MIME siganture
#APP_MIME_SIG = x-vnd.Haiku-BeShare

#	add support for new Pe and Eddie features
#	to fill in generic makefile

NASM = nasm  

#%{
# @src->@ 

#	specify the source files to use
#	full paths or paths relative to the makefile can be included
# 	all files, regardless of directory, will have their object
#	files created in the common object directory.
#	Note that this means this makefile will not work correctly
#	if two source files with the same name (source.c or source.cpp)
#	are included from different directories.  Also note that spaces
#	in folder names do not work well with this makefile.
SRCS += internal/atime.cpp
SRCS += internal/Cursors.cpp
SRCS += internal/direct_convert.cpp
SRCS += internal/s9x_key_names.cpp
SRCS += internal/s9x_screenshot.cpp
SRCS += internal/s9x_settings.cpp

SRCS += rominfo/S9xRomInfo.cpp

SRCS += settings/SettingsWindow.cpp
SRCS += settings/SettingTabView.cpp
SRCS += settings/DisplayView.cpp
SRCS += settings/EmulatorView.cpp 
SRCS += settings/SoundView.cpp
SRCS += settings/InputView.cpp

SRCS += controll/s9x_config_window.cpp
SRCS += controll/s9x_input_view.cpp
SRCS += controll/s9x_string_view.cpp 
SRCS += refresh/S9xRefreshWindow.cpp

SRCS += DisplayModeColumnListView.cpp 
SRCS += DisplayModeField.cpp
SRCS += GameStick.cpp
SRCS += Joystick.cpp
SRCS += GameMenuBar.cpp
SRCS += S9xApp.cpp
SRCS += S9xMainWindow.cpp
SRCS += S9xView.cpp
SRCS += s9x_flags.cpp
SRCS += S9xPortPart.cpp

SRCS += ../2xsai.cpp 
SRCS += ../apu.cpp 
SRCS += ../apudebug.cpp 
SRCS += ../c4.cpp
SRCS += ../c4emu.cpp
SRCS += ../cheats.cpp
SRCS += ../cheats2.cpp
SRCS += ../clip.cpp
SRCS += ../cpu.cpp
SRCS += ../cpuexec.cpp
SRCS += ../cpuops.cpp
SRCS += ../data.cpp
SRCS += ../debug.cpp
SRCS += ../dma.cpp
SRCS += ../dsp1.cpp
SRCS += ../fxemu.cpp
SRCS += ../fxinst.cpp
SRCS += ../fxdbg.cpp
SRCS += ../gfx.cpp
SRCS += ../globals.cpp 
SRCS += ../loadzip.cpp
SRCS += ../memmap.cpp
SRCS += ../movie.cpp
SRCS += ../obc1.cpp 
SRCS += ../ppu.cpp
SRCS += ../sa1.cpp
SRCS += ../sa1cpu.cpp
SRCS += ../screenshot.cpp 
SRCS += ../sdd1.cpp
SRCS += ../sdd1emu.cpp
SRCS += ../seta.cpp
SRCS += ../seta010.cpp 
SRCS += ../seta011.cpp
SRCS += ../seta018.cpp
SRCS += ../snaporig.cpp
SRCS += ../snapshot.cpp 
SRCS += ../snes9x.cpp
SRCS += ../soundux.cpp
SRCS += ../spc700.cpp
SRCS += ../spc7110.cpp 
SRCS += ../srtc.cpp 
SRCS += ../tile.cpp

SRCS += ../unzip/explode.c
SRCS += ../unzip/unreduce.c
SRCS += ../unzip/unshrink.c
SRCS += ../unzip/unzip.c 

SRCS += ../jma/7zlzma.cpp
SRCS += ../jma/crc32.cpp
SRCS += ../jma/iiostrm.cpp
SRCS += ../jma/inbyte.cpp 
SRCS += ../jma/jma.cpp
SRCS += ../jma/lzma.cpp
SRCS += ../jma/lzmadec.cpp 
SRCS += ../jma/s9x-jma.cpp
SRCS += ../jma/winout.cpp

#SRCS += libhaiku/ColorTools.cpp
#SRCS += libhaiku/ColumnListView.cpp
#SRCS += libhaiku/ColumnTypes.cpp

#SRCS += ../i386/SPC.ASM
#SRCS += ../i386/C4.ASM
#SRCS += ../i386/2XSAIMMX.ASM
#SRCS += ../i386/bilinear.asm
#SRCS += ../i386/fxemu2.asm
#SRCS += ../i386/fxemu2b.asm
#SRCS += ../i386/fxemu2c.asm
#SRCS += ../i386/fxtable.asm
#SRCS += ../i386/ZSNES.ASM
#SRCS += ../i386/sfxproc.asm
#SRCS += ../i386/zsnesc4.asm

# CPUOBJ=i386/cpuops.o i386/cpuexec.o i386/sa1ops.o ?


#	specify the resource definition files to use
#	full path or a relative path to the resource file can be used.
#RDEFS = BeShare.rdef
	
#	specify the resource files to use. 
#	full path or a relative path to the resource file can be used.
#	both RDEFS and RSRCS can be defined in the same makefile.
RSRCS = Resource.rsrc

#Check whats in use, remove those that don't
LIBS += be stdc++.r4 localestub columnlistview z
LIBS += device game media tracker translation
#LIBS += root network

#	specify additional paths to directories following the standard
#	libXXX.so or libXXX.a naming scheme.  You can specify full paths
#	or paths relative to the makefile.  The paths included may not
#	be recursive, so include all of the paths where libraries can
#	be found.  Directories where source files are found are
#	automatically included.
LIBPATHS += /boot/system/lib

#	additional paths to look for system headers
#	thes use the form: #include <header>
#	source file directories are NOT auto-included here
SYSTEM_INCLUDE_PATHS += /boot/system/develop/headers/private/interface
SYSTEM_INCLUDE_PATHS += /boot/system/develop/headers/os
#SYSTEM_INCLUDE_PATHS += /boot/system/develop/headers/cpp
#SYSTEM_INCLUDE_PATHS += /boot/system/develop/headers/posix

#	additional paths to look for local headers
#	thes use the form: #include "header"
#	source file directories are automatically included
LOCAL_INCLUDE_PATHS = #SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x SRCS += ../snes9x internal rominfo settings controll refresh libhaiku ../ internal controll libhaiku rominfo settings refresh

#	specify the level of optimization that you desire
#	NONE, SOME, FULL
OPTIMIZE = FULL

# 	specify here the codes for languages you are going
# 	to support in this application. The default "en"
# 	one must be provided too. "make catkeys" will recreate only
# 	locales/en.catkeys file. Use it as template for creating other
# 	languages catkeys. All localization files must be placed
# 	in "locales" sub-directory.
#LOCALES = 

#	specify any preprocessor symbols to be defined.  The symbols will not
#	have their values set automatically; you must supply the value (if any)
#	to use.  For example, setting DEFINES to "DEBUG=1" will cause the
#	compiler option "-DDEBUG=1" to be used.  Setting DEFINES to "DEBUG"
#	would pass "-DDEBUG" on the compiler's command line.
DEFINES += DEBUG
DEFINES += CPU_SHUTDOWN
DEFINES += JMA_SUPPORT
DEFINES += JOYSTICK_SUPPORT
DEFINES += MITSHM
DEFINES += NETPLAY_SUPPORT
DEFINES += NO_INLINE_SET_GET
DEFINES += SPC700_SHUTDOWN
DEFINES += SDD1_DECOMP
DEFINES += SPC700_C
DEFINES += USE_THREADS
DEFINES += UNZIP_SUPPORT
DEFINES += VAR_CYCLES
DEFINES += ZLIB 
#DEFINES += EXECUTE_SUPERFX_PER_LINE
#using ASM files
#DEFINES += MMX
#DEFINES += ZSNES_C4
#DEFINES += ZSNES_FX

#	specify special warning levels
#	if unspecified default warnings will be used
#	NONE = supress all warnings
#	ALL = enable all warnings
WARNINGS = ALL

#	specify whether image symbols will be created
#	so that stack crawls in the debugger are meaningful
#	if TRUE symbols will be created
SYMBOLS = 

#	specify debug settings
#	if TRUE will allow application to be run from a source-level
#	debugger.  Note that this will disable all optimzation.
DEBUGGER =

#	specify additional compiler flags for all files
COMPILER_FLAGS=-funsigned-bitfields -Wwrite-strings -O6 -march=pentium -fomit-frame-pointer -fno-exceptions -Wall -pipe -fexceptions
#COMPILER_FLAGS=-funsigned-bitfields -Wwrite-strings -DZLIB -DJOYSTICK_SUPPORT -DZSNES_C4 -DSPC700_C -DZSNES_FX -DMMX -DUNZIP_SUPPORT -DEXECUTE_SUPERFX_PER_LINE -DVAR_CYCLES -DNO_INLINE_SET_GET -DCPU_SHUTDOWN -DSPC700_SHUTDOWN -DUSE_THREADS -O6 -march=pentium -fomit-frame-pointer -fno-exceptions -Wall -pipe -DJMA_SUPPORT -fexceptions -DSDD1_DECOMP -DDEBUG -DMITSHM -DNETPLAY_SUPPORT
#COMPILER_FLAGS =

#	specify additional linker flags
LINKER_FLAGS =

#	specify the version of this particular item
#	(for example, -app 3 4 0 d 0 -short 340 -long "340 "`echo -n -e '\302\251'`"1999 GNU GPL") 
#	This may also be specified in a resource.
APP_VERSION = 

#	(for TYPE == DRIVER only) Specify desired location of driver in the /dev
#	hierarchy. Used by the driverinstall rule. E.g., DRIVER_PATH = video/usb will
#	instruct the driverinstall rule to place a symlink to your driver's binary in
#	~/add-ons/kernel/drivers/dev/video/usb, so that your driver will appear at
#	/dev/video/usb when loaded. Default is "misc".
DRIVER_PATH = 

CATKEYS_DIR = locales

## include the makefile-engine
DEVEL_DIRECTORY := \
	$(shell findpaths -r "makefile_engine" B_FIND_PATH_DEVELOP_DIRECTORY)
include $(DEVEL_DIRECTORY)/etc/makefile-engine
