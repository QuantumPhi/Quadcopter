typedef struct
{
    double value; //Estimate
    double covariance; //Error Covariance
    double noise; //Environment noise
} Kalman;

public static Kalman* init(double noise);
public static double update(Kalman* kalman, double data);
