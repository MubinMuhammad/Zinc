#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void zinc_blur_gaussian_function(float kernel[3][3], int size, int sigma) {
  for (int i = -1; i < 1; i++) {
    for (int j = -1; j < 1; j++) {
      kernel[i+1][j+1] = (1/(2 * 3.1416 * sigma * sigma))
                     * exp(-((float)j*(float)j + (float)i*(float)i) 
                     / (2 * sigma * sigma));
    }
  }
}

int main() {
  float kernel[3][3];

  zinc_blur_gaussian_function(kernel, 3, 1);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) 
      printf("%.3f ", kernel[i][j]);
    printf("\n");
  }

  return 0;
}
