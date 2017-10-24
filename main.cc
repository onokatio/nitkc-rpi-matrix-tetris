#include "led-matrix.h"

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>
#include <vector>
#include <iostream>

using namespace std;
using rgb_matrix::GPIO;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

class Fallblock {
	private:
	public:
		const static int width = 3;
		const static int height = 3;
		int x,y;
		vector<vector<int> > map = vector<vector<int> >(width,vector<int>(height,0));
};
class Tetris {
	private:
	public:
		const static int width = 32;
		const static int height = 32;
		vector<vector<int> > map = vector<vector<int> >(width,vector<int>(height,0));
		Fallblock *fall = new Fallblock;
		Tetris(){
			fall->map[0][0] = 1;
			cout << 1;
		}
};

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
  interrupt_received = true;
}

static void DrawOnCanvas(Canvas *canvas) {
  int x,y;

	while(!interrupt_received){
 	 	    canvas->SetPixel(0,0,255, 255, 255);
 	 	    usleep(1 * 1000 * 1000);
 	 	    canvas->SetPixel(0,0,0, 0, 0);
 	 	    usleep(1 * 1000 * 1000);
	}
}

int main(int argc, char *argv[]) {
  RGBMatrix::Options defaults;
  defaults.hardware_mapping = "regular";
  defaults.rows = 32;
  defaults.chain_length = 1;
  defaults.parallel = 1;
  //defaults.show_refresh_rate = true;
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
