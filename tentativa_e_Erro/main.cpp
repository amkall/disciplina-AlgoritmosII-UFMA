#include<iostream>
using namespace std;

int main(){
 int BLACK, WHITE, threshold, x, y;

 double error, a;
 BLACK = 0;
 WHITE = 16;

 float pixel[5][5] = {{15,7,15,2,7}, {5,5,7,1,13}, {6,12,5,1,14}, {2,8,9,12,15}, {1,9,10,4,15}};

 threshold = (WHITE + BLACK) / 2;
 /* esquerda para direita de cima para baixo */
 for( y = 0; y <= 4; y++ ) {
     for( x = 0; x <= 4; x++ ){
         if( pixel[x][y] < threshold ) {
         error = pixel[x][y] - BLACK;
         pixel[x][y] = BLACK;
     }
     else {
         error = pixel[x][y] - WHITE;
         pixel[x][y] = WHITE;
     }
     a = ((3/8) * error) * 100;
    pixel[x+1][y] += (3/8) * error;
    pixel[x][y+1] += (3/8) * error;
    pixel[x+1][y+1] +=  (2/8) *error;
    }
  }
  for(x = 0; x<5;x++){
    for(y = 0 ; y<5;y++){
        printf("%1.f", pixel[x][y]);
        cout<<" ";
    }
    cout<<"\n";
  }
}
