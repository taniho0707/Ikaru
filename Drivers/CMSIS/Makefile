##
# Makefile to build DSP_Lib into libraries
# Taken from: https://github.com/mikeferguson/stm32/blob/9e27786de4e6df74dc735f3c5667fbe4874e59c5/libraries/CMSIS/Makefile
##

ARCH        := M4
LIB         := DSP_Lib/libarm_cortex$(ARCH)lfsp_math.a

# Architectures to build for
ARCHS       := M4
# M0l_FLAGS   := -mthumb -mcpu=cortex-m0 -DARM_MATH_CM0
# M3l_FLAGS   := -mthumb -mcpu=cortex-m3 -DARM_MATH_CM3
# M4l_FLAGS   := -mthumb -mcpu=cortex-m4 -DARM_MATH_CM4
M4lf_FLAGS  := -mthumb -mcpu=cortex-m4 -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -DARM_MATH_CM4 -D__FPU_PRESENT

# Directories
DSPLIB      := DSP_Lib/Source
DSPDIRS     := BasicMathFunctions          \
               CommonTables                \
               ComplexMathFunctions        \
               ControllerFunctions         \
               FastMathFunctions           \
               FilteringFunctions          \
               MatrixFunctions             \
               StatisticsFunctions         \
               SupportFunctions            \
               TransformFunctions

# Build Paths
BUILD_PATH  := DSP_Lib_Build
BUILDDIRS   := $(BUILD_PATH)
BUILDDIRS   += $(foreach dir,$(DSPDIRS),$(BUILD_PATH)/$(DSPLIB)/$(dir))

# Targets to build
LIBS        := $(foreach tgt,$(ARCHS),Lib/libarm_cortex$(tgt)lfsp_math.a)
SRCS        := $(wildcard $(DSPLIB)/*/*.c)
OBJS        := $(patsubst %.c, %.o, $(SRCS))
OBJS        := $(foreach obj,$(OBJS),$(BUILD_PATH)/$(obj))

# Tools
CC  := /usr/bin/arm-none-eabi-gcc
CXX := /usr/bin/arm-none-eabi-g++
LD	:= /usr/bin/arm-none-eabi-ld
AR	:= /usr/bin/arm-none-eabi-ar
AS	:= /usr/bin/arm-none-eabi-gcc
CP	:= /usr/bin/arm-none-eabi-objcopy
OD	:= /usr/bin/arm-none-eabi-objdump
SIZE:= /usr/bin/arm-none-eabi-size

# Flags
INCLUDES    := -I./Include
CFLAGS      := -c -fno-common -g3 -Os -ffunction-sections $(M4lf_FLAGS)
CXXFLAGS    := $(CFLAGS) -fno-exceptions
CPFLAGS     := -Obinary -v
ODFLAGS     := -S -t
ARFLAGS     := 


all: $(BUILDDIRS) $(OBJS) $(LIB)
clean:
	rm -rf $(BUILD_PATH)

$(BUILD_PATH)/%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $($(ARCH)_FLAGS) -o $@ $<

$(LIB): $(OBJS)
	$(AR) rcs $@ $?

$(BUILDDIRS):
	@mkdir -p $@
