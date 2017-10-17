#include "led-matrix.h"

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>

#define FallblockX 3
#define FallblockY 3

using rgb_matrix::GPIO;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

class Fallblock{
	private:
		int x,y;
		int map[FallblockX][FallblockY] {};
	public:
		void showMap(){
			for
		}
};

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
  interrupt_received = true;
}

static void DrawOnCanvas(Canvas *canvas) {
  int x,y;

	while(!interrupt_received){
 	 	    canvas->SetPixel(1,1,255, 255, 255);
 	 	    usleep(1 * 1000 * 1000);
 	 	    canvas->SetPixel(1,1,0, 0, 0);
 	 	    usleep(1 * 1000 * 1000);
	}
}

int main(int argc, char *argv[]) {
  RGBMatrix::Options defaults;
  defaults.hardware_mapping = "regular";
  defaults.rows = 32;
  defaults.chain_length = 1;
  defaults.parallel = 1;
  defaults.show_refresh_rate = true;
  Canvas *canvas = rgb_matrix::CreateMatrixFromFlags(&argc, &argv, &defaults);
  if (canvas == NULL)
    return 1;

  signal(SIGTERM, InterruptHandler);
  signal(SIGINT, InterruptHandler);

  DrawOnCanvas(canvas);    // Using the canvas.

  canvas->Clear();
  delete canvas;

  return 0;
}
