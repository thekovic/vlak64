SOURCE_DIR = src
BUILD_DIR = build

include $(N64_INST)/include/n64.mk

# A for NTSC, P for PAL
N64_ROM_REGION="A"
N64_ROM_TITLE = "Vlak64 Zen C Edition"

src = vlak_main.c

all: vlak64.z64

ASSETS_SPRITES = $(wildcard assets/*.png)
ASSETS_SFX = $(wildcard assets/*.wav)

FS_SPRITES = $(addprefix filesystem/, $(notdir $(ASSETS_SPRITES:%.png=%.sprite)))
FS_SFX = $(addprefix filesystem/, $(notdir $(ASSETS_SFX:%.wav=%.wav64)))

filesystem/%.sprite: assets/%.png
	@mkdir -p $(dir $@)
	@echo "    [MKSPRITE] $< -> $@"
	@$(N64_MKSPRITE) -f RGBA16 -o "$(dir $@)" "$<"

filesystem/%.wav64: assets/%.wav
	@mkdir -p $(dir $@)
	@echo "    [AUDIOCONV64 (SFX)] $< -> $@"
	@$(N64_AUDIOCONV) --wav-compress 0 -o "$(dir $@)" "$<"

$(BUILD_DIR)/vlak64.dfs: $(FS_SPRITES) $(FS_SFX)

C_FILES := $(shell find $(SOURCE_DIR)/ -type f -name '*.c' | sort)
ZC_FILES := $(shell find $(SOURCE_DIR)/ -type f -name '*.zc' | sort)
ZC_TO_C_FILES := $(ZC_FILES:$(SOURCE_DIR)/%.zc=$(SOURCE_DIR)/%.c)
OBJECT_FILES := $(C_FILES:$(SOURCE_DIR)/%.c=$(BUILD_DIR)/%.o) \
				$(ZC_FILES:$(SOURCE_DIR)/%.zc=$(BUILD_DIR)/%.o)

$(SOURCE_DIR)/%.c: $(SOURCE_DIR)/%.zc
	zc transpile --cc mips64-elf-gcc -o $@ $<

$(BUILD_DIR)/vlak64.elf: $(OBJECT_FILES)

vlak64.z64: $(BUILD_DIR)/vlak64.dfs

clean:
	rm -rf $(BUILD_DIR) vlak64.z64 $(ZC_TO_C_FILES)

clean_fs:
	rm -rf filesystem/

-include $(wildcard $(BUILD_DIR)/*.d)

.PHONY: all clean
