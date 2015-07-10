CXXFLAGS = -I./include -Wall -DLINUXPC -Wno-deprecated -Wno-write-strings
#CXXFLAGS = -I./include -Wall -DLINUXPC -Wno-deprecated -Wno-write-strings -fopenmp

OBJECTS=Ymain_shalw.o Yshalw.o
CXXFLAGS += -DDEBUG -g -pg
TARGET=shalw

ifeq ($(OPTIM), yes)
CXXFLAGS += -m64 -O3 -fno-builtin #-msse2  -ftree-vectorizer-verbose=5 
endif
all: $(TARGET)

$(OBJECTS): Ymain_shalw.cpp Yshalw.cpp shalw.h Y1shalw.h Y2shalw.h ProjectShalw.hpp ProjectShalw.hpp Hfil.h Hphy.h Ufil.h Uphy.h Vfil.h Vphy.h
	$(CXX) $(CXXFLAGS) $(DEFINE) -c Ymain_shalw.cpp Yshalw.cpp

$(TARGET):$(OBJECTS)
	$(CXX) $(CXXFLAGS) $(DEFINE) -o$(TARGET) $(OBJECTS) $(MQN)

.PHONY: clean

clean:
	rm -f *.o; rm -f shalw

