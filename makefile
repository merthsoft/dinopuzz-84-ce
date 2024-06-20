# ----------------------------
# Makefile Options
# ----------------------------

NAME = DINOPUZZ
ICON = src/gfx/icon_modern.png
DESCRIPTION = "Dino Puzzle"
COMPRESSED = YES
ARCHIVED = NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
