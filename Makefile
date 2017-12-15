TARGET_EXEC ?= affectapp

BUILD_DIR ?= ./build

CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

#$(CFLAGS) $(LIBS)

all:
	g++ -std=c++11 -I/home/pi/workspace/affectiva/include -I/home/pi/workspace/plog/include/ -L/home/pi/workspace/affectiva/lib -l affdex-native src/prueba.cpp -o $(TARGET_EXEC)


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
