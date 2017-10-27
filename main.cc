#include "led-matrix.h"
#include "threaded-canvas-manipulator.h"

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>
#include <vector>
#include <iostream>
#include <sys/time.h>

using namespace std;
using rgb_matrix::GPIO;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;
using namespace rgb_matrix;

extern "C"{
	#include "niusb6501.h"
}
class Fallblock {
	private:
	public:
		const static int width = 3;
		const static int height = 3;
		int x = 0;
		int y = 0;
		int prev_color = 0;
		//vector<vector<int> > map = vector<vector<int> >(width,vector<int>(height,0));
		int map[3][3] = {};
		Fallblock(){
			createBlock();
		}
		void createBlock(){
			srand((unsigned int)time(NULL));
			x = 0;
			y = 0;
			int c = prev_color % 4 + 1;
			prev_color++;
			switch(rand() % 5){
				case 0:
					this->map[0][0] = c; this->map[1][0] = c; this->map[2][0] = c;
					this->map[0][1] = 0; this->map[1][1] = c; this->map[2][1] = 0;
					this->map[0][2] = 0; this->map[1][2] = c; this->map[2][2] = 0;
					break;
				case 1:
					this->map[0][0] = 0; this->map[1][0] = c; this->map[2][0] = 0;
					this->map[0][1] = c; this->map[1][1] = c; this->map[2][1] = c;
					this->map[0][2] = 0; this->map[1][2] = c; this->map[2][2] = 0;
					break;
				case 2:
					this->map[0][0] = c; this->map[1][0] = c; this->map[2][0] = 0;
					this->map[0][1] = c; this->map[1][1] = c; this->map[2][1] = 0;
					this->map[0][2] = 0; this->map[1][2] = 0; this->map[2][2] = 0;
					break;
				case 3:
					this->map[0][0] = c; this->map[1][0] = 0; this->map[2][0] = 0;
					this->map[0][1] = c; this->map[1][1] = 0; this->map[2][1] = 0;
					this->map[0][2] = c; this->map[1][2] = c; this->map[2][2] = c;
					break;
				case 4:
					this->map[0][0] = c; this->map[1][0] = 0; this->map[2][0] = 0;
					this->map[0][1] = c; this->map[1][1] = 0; this->map[2][1] = 0;
					this->map[0][2] = c; this->map[1][2] = 0; this->map[2][2] = 0;
					break;
				case 5:
					this->map[0][0] = c; this->map[1][0] = 0; this->map[2][0] = 0;
					this->map[0][1] = c; this->map[1][1] = c; this->map[2][1] = 0;
					this->map[0][2] = 0; this->map[1][2] = c; this->map[2][2] = 0;
					break;
			}
		}
		void roll90(){
			int old_map[3][3] = {};
			for(int x = 0 ; x < 3 ; x++ ){
				for(int y = 0 ; y < 3 ; y++ ){
					old_map[x][y] = map[x][y];
				}
			}
			for(int x = 0 ; x < 3 ; x++ ){
				for(int y = 0 ; y < 3 ; y++ ){
					map[x][y] = old_map[2-y][x];
				}
			}
		}
};
class Tetris : public ThreadedCanvasManipulator {
	private:
	public:
		const static int width = 32;
		const static int height = 32;
		int map[32][32] = {};
		int map_hold[32][32] = {};
		Fallblock *fall = new Fallblock;
		int frame = 0;
		time_t prev_time = time(0);
		
		Tetris(Canvas *canvas) : ThreadedCanvasManipulator(canvas) {
			this->prev_time = time(0);
		}
		void half_sec(){
			if( frame > 5 ){
				if( rand() % 5 == 0) fall->roll90();
				checkFall();
				frame = 0;
			}
		}
		virtual void Run(){
			while(running()){
				frame++;
				clearMap();
				canvas()->Clear();
				drawFallblock();
				drawHoldblock();
				draw();
				half_sec();
				move();
				usleep(15 * 1000);
			}
		}
		void move(){
			unsigned char read = read_port(2);
			cout << int(read) << endl;
		}
		void checkFall(){
			int stop = 0;
			for( int x = 0; x < 3 ; x++ ){
				for( int y = 0; y < 3 ; y++ ){
					if( fall->map[x][y] != 0 ){
						if( map_hold[ fall->x + x ][ fall->y + y +1 ] != 0){
							stop = 1;
						}
						if( (fall->y+1 + y) >= 32 ){
							stop = 1;
						}
					}
				}
			}
			if( stop == 0 ){
				fall->y++;
			}else{
				if( fall->y == 0 ){
					cout << "Game over";
					exit(0);
				}else{
					FallToHoldblock();
					fall->createBlock();
					stop = 0;
				}
			}
		}
		void draw(){
				for(int x = 0; x < 32 ; x++){
					for(int y = 0; y < 32 ; y++){
						switch(this->map[x][y]){
							case 1:
								canvas()->SetPixel(x, y, 255, 255, 255); break;
							case 2:
								canvas()->SetPixel(x, y, 255,   0,   0); break;
							case 3:
								canvas()->SetPixel(x, y,   0, 255,   0); break;
							case 4:
								canvas()->SetPixel(x, y,   0,   0, 255); break;
						}
						//cout << map[x][y];
					}
					//cout << endl;
				}
		//cout << endl;
		}
		void drawFallblock(){
			for(int x = 0; x < 3 ; x++ ){
				for(int y = 0; y < 3 ; y++ ){
					this->map[ fall->x + x ][ fall->y + y ] |= this->fall->map[x][y];
				}
			}
		}
		void drawHoldblock(){
			for(int x = 0; x < 32 ; x++ ){
				for(int y = 0; y < 32 ; y++ ){
					this->map[x][y] |= this->map_hold[x][y];
				}
			}
		}
		void FallToHoldblock(){
			for(int x = 0; x < 3 ; x++ ){
				for(int y = 0; y < 3 ; y++ ){
					this->map_hold[ fall->x + x ][ fall->y + y ] |= this->fall->map[x][y];
				}
			}
			for(int x = 0; x < 32 ; x++ ){
				for(int y = 0; y < 32 ; y++ ){
					//cout << map_hold[x][y];
				}
				//cout << endl;
			}
		}
		void clearMap(){
			for(int x = 0; x < this->width ; x++ ){
				for(int y = 0; y < this->width ; y++ ){
					this->map[x][y] = 0;
				}
			}
		}
};

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
  interrupt_received = true;
}

int main(int argc, char *argv[]) {
	init_device();
	set_io_mode(0x00,0x00,0x00);

	signal(SIGTERM, InterruptHandler);
	signal(SIGINT, InterruptHandler);

  RGBMatrix::Options matrix_options;
	rgb_matrix::RuntimeOptions runtime_opt;

	matrix_options.hardware_mapping = "regular";
  matrix_options.rows = 32;
  matrix_options.chain_length = 1;
  matrix_options.parallel = 1;
  matrix_options.show_refresh_rate = false;
	matrix_options.rows = 32;

	
	RGBMatrix *matrix = CreateMatrixFromOptions(matrix_options, runtime_opt);
	Tetris *tetris = new Tetris(matrix);
	tetris->Start();

	while(!interrupt_received){}

	tetris->Stop();

  return 0;
}
