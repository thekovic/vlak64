SOURCE_DIR = src
BUILD_DIR = build

include $(N64_INST)/include/n64.mk

# A for NTSC, P for PAL
N64_ROM_REGION="A"
N64_ROM_TITLE = "Vlak 64"

src = vlak_main.c

all: vlak64.z64

ASSETS_SPRITES = $(wildcard assets/*.png)
ASSETS_SFX = $(wildcard assets/*.wav)
ASSETS_MUSIC = $(wildcard assets/*.xm)

FS_SPRITES = $(addprefix filesystem/, $(notdir $(ASSETS_SPRITES:%.png=%.sprite)))
FS_SFX = $(addprefix filesystem/, $(notdir $(ASSETS_SFX:%.wav=%.wav64)))
FS_MUSIC = $(addprefix filesystem/, $(notdir $(ASSETS_MUSIC:%.xm=%.xm64)))

filesystem/%.sprite: assets/%.png
	@mkdir -p $(dir $@)
	@echo "    [MKSPRITE] $< -> $@"
	@$(N64_MKSPRITE) -f RGBA16 -o "$(dir $@)" "$<"

filesystem/%.wav64: assets/%.wav
	@mkdir -p $(dir $@)
	@echo "    [AUDIOCONV64 (SFX)] $< -> $@"
	@$(N64_AUDIOCONV) --wav-compress 0 -o "$(dir $@)" "$<"

filesystem/%.xm64: assets/%.xm
	@mkdir -p $(dir $@)
	@echo "    [AUDIOCONV64 (MUSIC)] $< -> $@"
	@$(N64_AUDIOCONV) -o "$(dir $@)" "$<"

$(BUILD_DIR)/vlak64.dfs: $(FS_SPRITES) $(FS_SFX) $(FS_MUSIC)

SOURCE_FILES := $(shell find $(SOURCE_DIR)/ -type f -name '*.c' | sort)
OBJECT_FILES := $(SOURCE_FILES:$(SOURCE_DIR)/%.c=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/vlak64.elf: $(OBJECT_FILES)

vlak64.z64: $(BUILD_DIR)/vlak64.dfs

clean:
	rm -rf $(BUILD_DIR) vlak64.z64

cleanfs:
	rm -rf filesystem/

-include $(wildcard $(BUILD_DIR)/*.d)

.PHONY: all clean
