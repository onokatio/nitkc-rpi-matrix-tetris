LED_LIBRARY_PATH=rpi-rgb-led-matrix/lib
LED_LIBS+=$(LED_LIBRARY_PATH)/librgbmatrix.a

all: $(LED_LIBS)
	g++ main.cc -o main -Irpi-rgb-led-matrix/include -Lrpi-rgb-led-matrix/lib -lrgbmatrix -lrt -lm -lpthread -std=c++11

$(LED_LIBS) :
	cd $(LED_LIBRARY_PATH) && make -j
