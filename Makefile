TARGET_EXEC ?= affectapp

BUILD_DIR ?= ./build

CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`


all:
	g++ src/prueba.cpp -o $(TARGET_EXEC) -std=c++11 $(CFLAGS) -I/home/pi/workspace/affectiva/include -I/home/pi/workspace/plog/include/ -L/home/pi/workspace/affectiva/lib -l affdex-native $(LIBS)


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
