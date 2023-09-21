#include <fenv.h>
#include <math.h>
#include <stdio.h>

#define M_PI 3.14159265358979323846
#define M_TWO_PI (M_PI * 2.0)
#define M_HALF_PI (M_PI * 0.5)

const double phase_inc = 0.002;
double phase = 0;

void Process()
{
  double pphase = fmod(phase, M_HALF_PI);
  printf("phase=%.3f pphase=%.3f\n", phase, pphase);

  if (pphase + phase_inc > M_HALF_PI) {
    printf("Inc\n");
  }

  phase += phase_inc;
  if (phase >= M_TWO_PI) {
    phase -= M_TWO_PI;
    puts("----------------");
  }
}

int main(void)
{
  size_t i = 0;
  while (i <) {
    Process();
    i++;
  }
}