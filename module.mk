
TARGET = fixGB_module.a
DEBUG = 0

platform = linux

BUILD_DIR = objs/$(platform)

ifeq ($(DEBUG),1)
   BUILD_DIR := $(BUILD_DIR)-dbg
endif

all: $(TARGET)

OBJS :=
OBJS +=apu.o
OBJS +=cpu.o
OBJS +=input.o
OBJS +=main.o
OBJS +=mbc.o
OBJS +=mem.o
OBJS +=ppu.o
OBJS +=interface.o

OBJS := $(addprefix $(BUILD_DIR)/,$(OBJS))

ifeq ($(DEBUG),1)
   FLAGS += -g -O0
else
   FLAGS += -O3
endif

FLAGS += -Wall -Wextra -Werror
FLAGS += -Wno-sign-compare
FLAGS += -Wno-misleading-indentation
FLAGS += -Wno-unused-parameter
FLAGS += -Wno-unused-variable
FLAGS += -Wno-unused-value
FLAGS += -Wno-unused-function
FLAGS += -Wno-unused-but-set-variable
FLAGS += -Wno-int-to-pointer-cast
FLAGS += -Werror=implicit-function-declaration -Werror=incompatible-pointer-types
FLAGS += -Wno-array-bounds
FLAGS += -fomit-frame-pointer

FLAGS += -msse -mfpmath=sse -ffast-math
FLAGS += -D__LIBRETRO__
FLAGS += -I.

CFLAGS += $(FLAGS)
CXXFLAGS += $(FLAGS) -fno-exceptions -fno-rtti


$(BUILD_DIR)/$(TARGET): $(OBJS) .lastbuild
	touch .lastbuild
	$(AR) rcs $@ $(OBJS)

$(TARGET): $(BUILD_DIR)/$(TARGET)
	cp $< $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $< $(CFLAGS) -MT $@ -MMD -MP -MF $(BUILD_DIR)/$*.depend -c -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $< $(CXXFLAGS) -MT $@ -MMD -MP -MF $(BUILD_DIR)/$*.depend -c -o $@

.lastbuild: ;

clean:
#	rm -rf objs
	rm -f $(OBJS) $(OBJS:.o=.depend)
	rm -f $(BUILD_DIR)/$(TARGET) $(TARGET) .lastbuild


-include $(OBJS:.o=.depend)
