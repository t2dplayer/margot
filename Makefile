vpath %.cpp src
vpath %.h include
TARGET = margot
CPPFLAGS = -I/usr/include -I./include
LDFLAGS = -lX11
GCC = g++ -g --std=c++2a

all: $(TARGET)

$(TARGET): main.o window.o 
	$(GCC) $^ -o $@ $(LDFLAGS)

%.o: %.cpp %.h
	$(GCC) -c $< $(CPPFLAGS)

clean:
	rm *.o $(TARGET)

run:
	./$(TARGET)

.PHONY: clean run
