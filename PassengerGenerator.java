import java.util.*;

class PassengerGenerator extends Thread {

    int current_time;
    public static double lambda;
    public static int next_arrival_time = 0;
    public static PassengerQueue queue1, queue2, queue3;
    public static int passenger_id = 0;
    public static int MAX_SIZE = 0;

    PassengerGenerator(int current_time) {
        this.current_time = current_time;
    }

    public void run() {
        if (this.current_time == next_arrival_time) {

            passenger_id++;
            Passenger new_passenger = new Passenger(passenger_id, this.current_time);

            int inter_arrival_time = Utility.getNextTime(lambda);
            next_arrival_time += inter_arrival_time;

            // System.out.println(passenger_id + " passenger generated at " + inter_arrival_time);
            // System.out.println(new_passenger);

            Random random = new Random();
            int officer_number = random.nextInt(3);
            if (officer_number == 0) {
                queue1.addPassenger(new_passenger, MAX_SIZE);
                // System.out.println("passenger added q1");
            }
            else if (officer_number == 1) {
                queue2.addPassenger(new_passenger, MAX_SIZE);
                // System.out.println("passenger added q2");
            }
            else {
                queue3.addPassenger(new_passenger, MAX_SIZE);
                // System.out.println("passenger added q3");
            }    
        }
    }
}