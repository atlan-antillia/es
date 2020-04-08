CC = g++
INC =  -I../../include  `pkg-config --cflags gtkmm-3.0` \
`pkg-config --cflags cairomm-1.0 ` \
`pkg-config --cflags opencv ` 

CFLAG = -O2 -std=c++11 -DGDK_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED

LIBDIR = -L../../lib

LIB = `pkg-config --libs gtkmm-3.0` \
`pkg-config --libs opencv` 
 
TARGET = ImageThresholding

target: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFALG) $(INC) -o $(TARGET) $(TARGET).cpp $(LIBDIR) $(LIB) $(LIB_OPENCV)

clean:
	rm -f *.o $(TARGET)

