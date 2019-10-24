#1. 指定此工程所需要头文件路径
INC_PATH = -I ./ -I ./inc
#  -lbreakpad
#2. 指定cpp 文件的目录

CXXFLAGS = -Wall -O2 -std=c++11 -fPIC $(INC_PATH)
LDXXFLAGS = $(LIBS) -pthread -shared




EXEC = libdev.so
GCC = g++
SUBDIR = . src
CPP_SOURCES = $(foreach dir, $(SUBDIR), $(wildcard $(dir)/*.cpp))
CPP_OBJECTS = $(patsubst %.cpp, %.o, $(CPP_SOURCES))
# DEP_FILES = $(patsubst %.o, %.d, $(CPP_OBJECTS))

$(EXEC): $(CPP_OBJECTS)
	$(GCC) $(CPP_OBJECTS) -o $@ $(LDXXFLAGS)

%.o: %.cpp
	$(GCC) -c $(CXXFLAGS) $< -o $@
# -include $(DEP_FILES)


clean:
	rm -rf $(CPP_OBJECTS) $(EXEC)