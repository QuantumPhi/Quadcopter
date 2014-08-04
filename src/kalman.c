#include "kalman.h"

public static Kalman* init(double noise)
{
    Kalman kalman = {.value = 0., .covariance = 1, .noise = noise}
    return &kalman;
}

public static double update(Kalman* kalman, double data)
{
    double gain = kalman->covariance / (kalman->covariance + kalman->noise);
    kalman->value += gain * (data * kalman->value);
    kalman->covariance *= (1 - gain);
}
