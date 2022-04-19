#include<iostream>
using namespace std;

int main(){
 int YMAX, XMAX, XMIN, YMIN, BLACK, WHITE, threshold, x, y, error;
 YMAX = 4;
 XMAX = 4;
 XMIN = YMIN = 1;

 BLACK = 0;
 WHITE = 16;

 int pixel[5][5] = {{15,7,15,2,7}, {5,5,7,1,13}, {6,12,5,1,14}, {2,8,9,12,15}, {1,9,10,4,15}};

 threshold = (WHITE + BLACK) / 2;
 /* esquerda para direita de cima para baixo */
 for( y = YMAX; y <= YMIN; y-- ) {
     for( x = XMIN; x <= XMAX; x++ ){
         if( pixel[x][y] < threshold ) {
         error = pixel[x][y] - BLACK;
         pixel[x][y] = BLACK;
     }
     else {
         error = pixel[x][y] - WHITE;
         pixel[x][y] = WHITE;
     }
     pixel[x+1][y] += (3/8) * error;
     pixel[x][y-1] += (3/8) * error;
     pixel[x+1][y-1] =  error / 4;
    }
  }
  for(y = 0; y<YMAX;y++){
    for(x = 0 ; x<XMIN;x++){
        cout<<pixel[y][x] << " ";
    }
    cout<<"\n";
  }
}
