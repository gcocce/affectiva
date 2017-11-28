TARGET_EXEC ?= affectapp

BUILD_DIR ?= ./build

all:
	g++ src/prueba.cpp -o $(TARGET_EXEC) -std=c++11 -I/home/pi/workspace/affectiva/include -I/home/pi/workspace/plog/include/ -L/home/pi/workspace/affectiva/lib -l affdex-native


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
