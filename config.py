import os

# User space START
BUILD_LEVEL = ''

TARGET_PLATFORM = 'linux'

LINK_SCRIPT = 'board/linker_scripts/link.lds' 

TARGET_NAME = 'program'

MY_EXEC_PATH = '/home/sogw/Downloads/gcc-arm-none-eabi-10-2020-q4-major-x86_64-linux/gcc-arm-none-eabi-10-2020-q4-major/bin/'
MY_EXEC_PATH += ':' '/mnt/d/ForLinux/gcc-arm-none-eabi-10-2020-q4-major/bin'
# User space END

#------
# The followings shouldn't be changed by user

#

TARGET = TARGET_NAME + '.elf'
HEX_TARGET = TARGET_NAME + '.hex'
BIN_TARGET = TARGET_NAME + '.bin'

#

target_platform = TARGET_PLATFORM
compiler_prefix = ''
link_script = ''
#* compiler related

CDEFINES = ['USE_HAL_DRIVER', 'STM32F103xB']

if target_platform == 'cortex-m3':
    compiler_prefix = 'arm-none-eabi-'
    link_script = LINK_SCRIPT
    #specific
    DEVICE = ' -mcpu=cortex-m3 -mthumb -ffunction-sections -fdata-sections'
    CCFLAGS = DEVICE + ' -Dgcc' + ' --specs=nano.specs'
    CPPDEFINES = CDEFINES
    ASFLAGS = ' -c' + DEVICE + ' -x assembler-with-cpp -Wa,-mimplicit-it=thumb '
    LINKFLAGS = DEVICE  + ' --specs=nano.specs' + ' -Wl,--gc-sections,-Map=target.map,-cref,-u,Reset_Handler' +  ' -T %s' % link_script

    LIBPATH = ''
    LIBS = []

    if BUILD_LEVEL == 'debug':
        CCFLAGS += ' -O0 -gdwarf-2 -g'
        ASFLAGS += ' -gdwarf-2'
    else:
        CCFLAGS += ' -Os'

    CXXFLAGS = CCFLAGS 

elif target_platform == 'linux':
    compiler_prefix = ''
    # link_script = '/home/vano/all/Desktop/latest/ua/tests/linux/link.lds'
    #specific
    CCFLAGS = ' -Dgcc' 
    CPPDEFINES = []
    ASFLAGS = ''
    LINKFLAGS = ' -Wl,-Map=target.map'  +  ' -W '
    # LINKFLAGS += ' %s' %link_script
    LIBPATH = ''
    LIBS = []

    if BUILD_LEVEL == 'debug':
        CCFLAGS += ' -O0 -gdwarf-2 -g'
        ASFLAGS += ' -gdwarf-2'
    else:
        CCFLAGS += ' -O0'

    CXXFLAGS = CCFLAGS
else:
    print("Unknown target-platform (%s)" %{target_platform})
    os.abort()


CC = compiler_prefix + 'gcc'
AS = compiler_prefix + 'gcc'
AR = compiler_prefix + 'ar'
CXX = compiler_prefix + 'g++'
LINK = compiler_prefix + 'gcc'
SIZE = compiler_prefix + 'size'
OBJDUMP = compiler_prefix + 'objdump'
OBJCPY = compiler_prefix + 'objcopy'

#

EXEC_PATH = MY_EXEC_PATH

#