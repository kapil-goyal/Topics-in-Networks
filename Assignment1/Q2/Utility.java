import java.util.*;

class Utility {
    public static int getNextTime(double lambda) {
        double p = Math.random();
        double inter_arrival_time = Math.round(Math.log(1-p) * (-1.0/lambda));
        return (int)(inter_arrival_time);
    }
}