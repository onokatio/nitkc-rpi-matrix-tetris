#include "led-matrix.h"

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>

using rgb_matrix::GPIO;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
  interrupt_received = true;
}

static void DrawOnCanvas(Canvas *canvas) {
  int x,y;
  canvas->Fill(0, 0, 0);

	while(!interrupt_received){
	  for(x=0;x < canvas->width(); x++){
 		  for(y=0;y < canvas->height(); y++){
  			canvas->Fill(0, 0, 0);
 	 	    canvas->SetPixel(x,y,255, 255, 255);
 	 	    usleep(1 * 1000 * 10);  // wait a little to slow down things.
 	 	  }
			x++;
 		  for(;y > -1; y--){
  			canvas->Fill(0, 0, 0);
 	 	    canvas->SetPixel(x,y,255, 255, 255);
 	 	    usleep(1 * 1000 * 10);  // wait a little to slow down things.
			}
 	 	}
  	canvas->Fill(0, 0, 0);
	}
}

int main(int argc, char *argv[]) {
  RGBMatrix::Options defaults;
  defaults.hardware_mapping = "regular";  // or e.g. "adafruit-hat"
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
