#ifndef GEOGEBRA_CONFIG_H
#define GEOGEBRA_CONFIG_H

#define Pixel_X_2_Frame_X(x,w) (-1.0f + 2 * (x) / (w))
#define Pixel_Y_2_Frame_Y(y,h) (1.0f - 2 * (y) / (h))

#define Frame_2_Pixel_X(x,w) ((x)+1.0f)*(w)/2
#define Frame_2_Pixel_Y(y,h) (1.0f-(y))*(h)/2

#define NOT_ADDING -1

#define ADD_CENTER 0
#define ADD_FIRST_POINT 0
#define POLYGON_ADDING 0

#define THRESHOLD 0.008f

#define PRECISION 0.00000000001f

#endif //GEOGEBRA_CONFIG_H
