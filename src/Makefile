TARGET   = vo
OBJECT   = visodo.o fast_hdmi.o
HEADERS	 = vo_features.h fast_hdmi.h
CC       = arm-xilinx-linux-gnueabi-g++
CFLAGS   = -O2 
INCLUDE_LOC = -I ~/opencv-3.1.0/install/include -I ~/opencv-3.1.0/install/include/opencv2 -L ~/opencv-3.1.0/install/lib
LIBS     =  -lm -lpthread -ldl -lrt -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d  -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core

all: $(TARGET)

%.o: %.cpp $(HEADERS)
	$(CC)  -c $< $(CFLAGS) $(INCLUDE_LOC) $(LIBS) $(HEADERS) -o $@

$(TARGET): $(OBJECT)
	$(CC) $^ -o $(TARGET) $(CFLAGS) $(INCLUDE_LOC) $(LIBS) 

.PHONY: clean

clean:
	$(RM) -f $(TARGET)
