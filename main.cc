#include "led-matrix.h"
#include "threaded-canvas-manipulator.h"

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
using namespace rgb_matrix;

class Fallblock {
	private:
	public:
		const static int width = 3;
		const static int height = 3;
		int x,y;
		//vector<vector<int> > map = vector<vector<int> >(width,vector<int>(height,0));
		int map[3][3] = {};
		Fallblock(){
			this->map[0][0] = 0; this->map[0][1] = 1; this->map[0][2] = 0;
			this->map[1][0] = 1; this->map[1][1] = 1; this->map[1][2] = 1;
			this->map[2][0] = 0; this->map[2][1] = 1; this->map[2][2] = 0;
		}
};
class Tetris : public ThreadedCanvasManipulator {
	private:
	public:
		const static int width = 32;
		const static int height = 32;
		//vector<vector<int> > map = vector<vector<int> >(32,vector<int>(32,1));
		int map[32][32] = {};
		//Canvas *canvas;
		Fallblock *fall = new Fallblock;
		
		Tetris(Canvas *canvas) : ThreadedCanvasManipulator(canvas) {}
		virtual void Run(){
			int red,green,blue;
			this->map[0][0] = 1; this->map[0][1] = 0; this->map[0][2] = 0;
			this->map[1][0] = 1; this->map[1][1] = 0; this->map[1][2] = 0;
			this->map[2][0] = 1; this->map[2][1] = 1; this->map[2][2] = 1;
			while(running()){
				for(int x = 0; x < 32 ; x++){
					for(int y = 0; y < 32 ; y++){
						switch(this->map[x][y]){
							case 0:
								red   = 0;
								green = 0;
								blue  = 0;
								break;
							case 1:
								red   = 255;
								green = 255;
								blue  = 255;
								break;
							case 2:
								red   = 255;
								green = 0;
								blue  = 0;
								break;
						}
						canvas()->SetPixel(y, x, red, green, blue);
					}
				}
				usleep(15 * 1000);
			}
		}
};

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
  interrupt_received = true;
}

static void DrawOnCanvas(Canvas *canvas) {
}

int main(int argc, char *argv[]) {
  RGBMatrix::Options matrix_options;
	rgb_matrix::RuntimeOptions runtime_opt;

  matrix_options.hardware_mapping = "regular";
  matrix_options.rows = 32;
  matrix_options.chain_length = 1;
  matrix_options.parallel = 1;
  matrix_options.show_refresh_rate = true;
	matrix_options.rows = 32;

	
	RGBMatrix *matrix = CreateMatrixFromOptions(matrix_options, runtime_opt);
	Tetris *tetris = new Tetris(matrix);
	tetris->Start();
	usleep(2 * 1000 * 1000);
	tetris->Stop();

  //canvas->Clear();
  //delete canvas;

  return 0;
}
