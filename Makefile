all:
	g++ main.cc -Irpi-rgb-led-matrix/include -Lrpi-rgb-led-matrix/lib -lrgbmatrix -lrt -lm -lpthread
