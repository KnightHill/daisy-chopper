#include <math.h>
#include <stdio.h>

//#define M_PI 3.14159265358979323846f
#define M_TWO_PI (M_PI * 2.0)
#define M_HALF_PI (M_PI * 0.5)

int main(void)
{
    for(float phase = 0; phase <= M_TWO_PI; phase += 0.05f)
        printf("phase=%f index=%d\n", phase, (unsigned int)floorf(phase / M_HALF_PI));
}
