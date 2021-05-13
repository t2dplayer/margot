vpath %.cpp src
vpath %.h include
TARGET = margot
CPPFLAGS = -I/usr/include -I./include
LDFLAGS = -lX11
GCC = g++ -g

all: $(TARGET)

$(TARGET): main.o window.o 
	$(GCC) --std=c++17 $^ -o $@ $(LDFLAGS)

%.o: %.cpp %.h
	$(GCC) -c $< $(CPPFLAGS)

clean:
	rm *.o $(TARGET)

run:
	./$(TARGET)

.PHONY: clean run
