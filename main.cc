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
		Fallblock(){
			this->map[0] = {0,1,0};
			this->map[1] = {0,1,0};
			this->map[2] = {1,1,1};
		}
};
class Tetris {
	private:
	public:
		const static int width = 32;
		const static int height = 32;
		vector<vector<int> > map = vector<vector<int> >(width,vector<int>(height,0));
		Canvas *canvas;
		Fallblock *fall = new Fallblock;
		
		Tetris(Canvas *canvas){
			//this->map = {};
			this->canvas = canvas;
			this->map[0][1] = 1;
			this->map[1][0] = 1;
			this->map[1][2] = 1;
			this->map[2][1] = 1;
			for(int i = 0; i < 2; i++){
				draw();
				usleep(1 * 1000 * 1000);
			}
		}
		void draw(){
			int red,green,blue;
			for(int x = 0; x < this->width ; x++){
				for(int y = 0; y < this->height ; y++){
					switch(this->fall->map[x][y]){
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
					//this->canvas->SetPixel(x, y, red, green, blue);
					this->canvas->SetPixel(x, y, 255, 255, 255);
					cout << x << " " << y << endl;
				}
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

  		//DrawOnCanvas(canvas);    // Using the canvas.

	Tetris *tetris = new Tetris(canvas);
  canvas->Clear();
  delete canvas;

  return 0;
}
