import java.util.*;

class Utility {
    public static int poisson(double mean) {
        Random random = new Random();
        int r = 0;
        double a = random.nextDouble();
        double p = Math.exp(-mean);
    
        while (a > p) {
            r++;
            a = a - p;
            p = p * mean / r;
        }
        return r;
    }

    public static int getPoissonRandom(double mean) {
        Random r = new Random();
        double L = Math.exp(-mean);
        int k = 0;
        double p = 1.0;
        do {
            p = p * r.nextDouble();
            k++;
        } while (p > L);
        return k - 1;
    }

    public static int getNextInterArrivalTime(double mean) {
        Random r = new Random();
        int next_inter_arrival_time = (int)(- Math.log(1.0 - r.nextDouble()) / mean);
        return next_inter_arrival_time;
    }
}