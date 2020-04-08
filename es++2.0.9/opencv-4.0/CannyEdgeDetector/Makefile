CC = g++

INC =  -I../../include  -I/usr/local/include/opencv4 `pkg-config --cflags gtkmm-3.0` \
`pkg-config --cflags cairomm-1.0 `

CFLAG = -O2 -std=c++11 -DGDK_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED

LIBDIR = -L../../lib -L/usr/local/lib

LIB = `pkg-config --libs gtkmm-3.0`

LIB_OPENCV= \
  -lopencv_calib3d \
  -lopencv_core \
  -lopencv_dnn \
  -lopencv_features2d \
  -lopencv_flann \
  -lopencv_gapi \
  -lopencv_highgui \
  -lopencv_imgcodecs \
  -lopencv_imgproc \
  -lopencv_ml \
  -lopencv_objdetect \
  -lopencv_photo \
  -lopencv_stitching \
  -lopencv_video \
  -lopencv_videoio


 
TARGET = CannyEdgeDetector

target: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAG) $(INC) -o $(TARGET) $(TARGET).cpp $(LIBDIR) $(LIB) $(LIB_OPENCV)

clean:
	rm -f *.o $(TARGET)

