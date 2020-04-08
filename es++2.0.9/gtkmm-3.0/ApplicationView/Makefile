CC = g++
INC =  -I../../include `pkg-config --cflags gtkmm-3.0` \
`pkg-config --cflags cairomm-1.0` 

CFLAG = -O2 -std=c++11 -Wformat-truncation=0
LIBDIR = -L../../lib
LIB = `pkg-config --libs gtkmm-3.0` `pkg-config --libs cairomm-1.0` 

TARGET = ApplicationView

target: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAG) $(INC) -o $(TARGET) $(TARGET).cpp $(LIBDIR) $(LIB) 

clean:
	rm -f *.o $(TARGET)

