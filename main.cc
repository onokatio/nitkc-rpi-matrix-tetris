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
		const static int width = 6;
		const static int height = 6;
		int real_width = 0;
		int real_height = 0;
		int x = 0;
		int y = 0;
		int prev_color = 0;
		//vector<vector<int> > map = vector<vector<int> >(width,vector<int>(height,0));
		int map[width][height] = {};
		Fallblock(){
			createBlock();
		}
		void createBlock(){
			srand((unsigned int)time(NULL));
			x = 0;
			y = 0;
			int c = prev_color % 6 + 1;
			prev_color++;
			switch(rand() % 5){
				case 0: // write I
					map[0][0] = c; map[1][0] = c; map[2][0] = c; map[3][0] = c; map[4][0] = c; map[5][0] = c;
					map[0][1] = c; map[1][1] = c; map[2][1] = c; map[3][1] = c; map[4][1] = c; map[5][1] = c;
					map[0][2] = 0; map[1][2] = 0; map[2][2] = 0; map[3][2] = 0; map[4][2] = 0; map[5][2] = 0;
					map[0][3] = 0; map[1][3] = 0; map[2][3] = 0; map[3][3] = 0; map[4][3] = 0; map[5][3] = 0;
					map[0][4] = 0; map[1][4] = 0; map[2][4] = 0; map[3][4] = 0; map[4][4] = 0; map[5][4] = 0;
					map[0][5] = 0; map[1][5] = 0; map[2][5] = 0; map[3][5] = 0; map[4][5] = 0; map[5][5] = 0;
					break;
				case 1: // write T
					map[0][0] = 0; map[1][0] = 0; map[2][0] = c; map[3][0] = c; map[4][0] = 0; map[5][0] = 0;
					map[0][1] = 0; map[1][1] = 0; map[2][1] = c; map[3][1] = c; map[4][1] = 0; map[5][1] = 0;
					map[0][2] = 0; map[1][2] = 0; map[2][2] = c; map[3][2] = c; map[4][2] = c; map[5][2] = c;
					map[0][3] = 0; map[1][3] = 0; map[2][3] = c; map[3][3] = c; map[4][3] = c; map[5][3] = c;
					map[0][4] = 0; map[1][4] = 0; map[2][4] = c; map[3][4] = c; map[4][4] = 0; map[5][4] = 0;
					map[0][5] = 0; map[1][5] = 0; map[2][5] = c; map[3][5] = c; map[4][5] = 0; map[5][5] = 0;
					break;
				case 2: //write rect
					map[0][0] = 0; map[1][0] = c; map[2][0] = c; map[3][0] = c; map[4][0] = c; map[5][0] = 0;
					map[0][1] = 0; map[1][1] = c; map[2][1] = c; map[3][1] = c; map[4][1] = c; map[5][1] = 0;
					map[0][2] = 0; map[1][2] = c; map[2][2] = c; map[3][2] = c; map[4][2] = c; map[5][2] = 0;
					map[0][3] = 0; map[1][3] = c; map[2][3] = c; map[3][3] = c; map[4][3] = c; map[5][3] = 0;
					map[0][4] = 0; map[1][4] = 0; map[2][4] = 0; map[3][4] = 0; map[4][4] = 0; map[5][4] = 0;
					map[0][5] = 0; map[1][5] = 0; map[2][5] = 0; map[3][5] = 0; map[4][5] = 0; map[5][5] = 0;
					break;
				case 3: // write L
					map[0][0] = 0; map[1][0] = 0; map[2][0] = c; map[3][0] = c; map[4][0] = 0; map[5][0] = 0;
					map[0][1] = 0; map[1][1] = 0; map[2][1] = c; map[3][1] = c; map[4][1] = 0; map[5][1] = 0;
					map[0][2] = 0; map[1][2] = 0; map[2][2] = c; map[3][2] = c; map[4][2] = 0; map[5][2] = 0;
					map[0][3] = 0; map[1][3] = 0; map[2][3] = c; map[3][3] = c; map[4][3] = 0; map[5][3] = 0;
					map[0][4] = 0; map[1][4] = 0; map[2][4] = c; map[3][4] = c; map[4][4] = c; map[5][4] = c;
					map[0][5] = 0; map[1][5] = 0; map[2][5] = c; map[3][5] = c; map[4][5] = c; map[5][5] = c;
					break;
				case 4: // write lightning
					map[0][0] = 0; map[1][0] = 0; map[2][0] = c; map[3][0] = c; map[4][0] = 0; map[5][0] = 0;
					map[0][1] = 0; map[1][1] = 0; map[2][1] = c; map[3][1] = c; map[4][1] = 0; map[5][1] = 0;
					map[0][2] = 0; map[1][2] = 0; map[2][2] = c; map[3][2] = c; map[4][2] = c; map[5][2] = c;
					map[0][3] = 0; map[1][3] = 0; map[2][3] = c; map[3][3] = c; map[4][3] = c; map[5][3] = c;
					map[0][4] = 0; map[1][4] = 0; map[2][4] = 0; map[3][4] = 0; map[4][4] = c; map[5][4] = c;
					map[0][5] = 0; map[1][5] = 0; map[2][5] = 0; map[3][5] = 0; map[4][5] = c; map[5][5] = c;
					break;
				case 5:
					map[0][0] = c; map[1][0] = c; map[2][0] = c; map[3][0] = 0;
					map[0][1] = 0; map[1][1] = c; map[2][1] = 0; map[3][1] = 0;
					map[0][2] = 0; map[1][2] = c; map[2][2] = 0; map[3][2] = 0;
					map[0][3] = 0; map[1][3] = c; map[2][3] = 0; map[3][3] = 0;
					break;
			}
		}
		void roll90(){
			int old_map[width][height] = {};
			for(int x = 0 ; x < width ; x++ ){
				for(int y = 0 ; y < height ; y++ ){
					old_map[x][y] = map[x][y];
				}
			}
			for(int x = 0 ; x < width ; x++ ){
				for(int y = 0 ; y < height ; y++ ){
					map[x][y] = old_map[(height-1)-y][x];
				}
			}
		}
		void roll90left(){
			roll90();
			roll90();
			roll90();
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
		int button[32] = {};
		
		Tetris(Canvas *canvas) : ThreadedCanvasManipulator(canvas) {
			this->prev_time = time(0);
			for(int y = 0; y < height ; y++ ){
				map_hold[20][y] = 1;
			}
		}
		void half_sec(){
			if( frame > 30 ){
				fall->y++;
				if( checkFall() == 1 ){
					fall->y--;
					FallToHoldblock();
					fall->createBlock();
				}
				frame = 0;
			}
		}
		virtual void Run(){
			while(running()){
				frame++;
				move();
				remove();
				clearMap();
				canvas()->Clear();
				draw();
				half_sec();
				usleep(15 * 1000);
			}
		}
		void remove(){
			int skip = 0;
			for(int y = 0; y < height ; y++ ){
				for(int x = 0; x < 20 ; x++ ){
					if(map[x][y] == 0){
						skip = 1;
					}
				}
				if(skip == 0){
					cout << y;
					for(int ny = y; ny > 0 ; ny-- ){
						for(int x = 0; x < 20 ; x++ ){
							map_hold[x][ny] = map_hold[x][ny-1];
						}
					}
				}else{
					skip = 0;
				}
			}
		}
		void move(){
			unsigned char read = read_port(2);
			for(int i = 0; i < 6 ; i++){
				if(~read & (0x01 << i)){
					if(button[i] > 80){
						button[i] = 1;
						switch(i){
							case 0: fall->x++;break;
							case 1: break;
							case 2: fall->x--;break;
							case 3: fall->roll90left();break;
							case 4: fall->y++;button[i] = 100;break;
							case 5: fall->roll90();break;
						}
						if(checkFall() == 1){
							switch(i){
								case 0: fall->x--;break;
								case 1: break;
								case 2: fall->x++;break;
								case 3: fall->roll90();break;
								case 4: fall->y--;break;
								case 5: fall->roll90left();break;
							}
						}
					}else{
						button[i]+=button[i];
					}
				}else{
					button[i] = 90;
				}
			}
		}
		int checkFall(){
			int stop = 0;
			for( int x = 0; x < fall->width ; x++ ){
				for( int y = 0; y < fall->height ; y++ ){
					if( fall->map[x][y] != 0 ){
						//if( map_hold[ fall->x + x ][ fall->y + y +1 ] != 0){
						if( map_hold[ fall->x + x ][ fall->y + y ] != 0){
							if(fall->y < 3){
								exit(0);
							}
							return 1;
						}
						if( (fall->y + y) >= height ){
							stop = 1;
							return 1;
						}
						if( (fall->x + x) >= width ){
							stop = 1;
							return 1;
						}
						if( (fall->x + x) < 0){
							stop = 1;
							return 1;
						}
					}
				}
			}
			if(stop == 1){
				if(fall->y == 0){
					cout << "game over" << endl;
					exit(0);
				}
				return 1;
			}
			return 0;
		}
		void draw(){
				drawFallblock();
				drawHoldblock();
				for(int x = 0; x < width ; x++){
					for(int y = 0; y < height ; y++){
						switch(this->map[x][y]){
							case 0:
								canvas()->SetPixel(x, y,   0,  0,   0); break;
							case 1:
								canvas()->SetPixel(x, y,   0,   0, 255); break;
							case 2:
								canvas()->SetPixel(x, y,   0, 255,   0); break;
							case 3:
								canvas()->SetPixel(x, y,   0, 255, 255); break;
							case 4:
								canvas()->SetPixel(x, y, 255,   0,   0); break;
							case 5:
								canvas()->SetPixel(x, y, 255,   0, 255); break;
							case 6:
								canvas()->SetPixel(x, y, 255, 255,   0); break;
							case 7:
								canvas()->SetPixel(x, y, 255, 255, 255); break;
						}
						//cout << map[x][y];
					}
					//cout << endl;
				}
		//cout << endl;
		}
		void drawFallblock(){
			for(int x = 0; x < fall->width ; x++ ){
				for(int y = 0; y < fall->height ; y++ ){
					this->map[ fall->x + x ][ fall->y + y ] |= this->fall->map[x][y];
				}
			}
		}
		void drawHoldblock(){
			for(int x = 0; x < width ; x++ ){
				for(int y = 0; y < height ; y++ ){
					this->map[x][y] |= this->map_hold[x][y];
				}
			}
		}
		void FallToHoldblock(){
			for(int x = 0; x < fall->width ; x++ ){
				for(int y = 0; y < fall->height ; y++ ){
					this->map_hold[ fall->x + x ][ fall->y + y ] |= this->fall->map[x][y];
				}
			}
			for(int x = 0; x < width ; x++ ){
				for(int y = 0; y < height ; y++ ){
					//cout << map_hold[x][y];
				}
				//cout << endl;
			}
		}
		void clearMap(){
			for(int x = 0; x < width ; x++ ){
				for(int y = 0; y < width ; y++ ){
					map[x][y] = 0;
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
