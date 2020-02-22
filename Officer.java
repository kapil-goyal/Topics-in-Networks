import java.util.*;

class Officer extends Thread {
    int current_time;
    int next_wakeup_time;
    int next_arrival_time;
    double mu;
    Passenger current_passenger;
    PassengerQueue passengers_queue;
    PassengerQueue serviced_queue;

    Officer(Passenger current_passenger,
            int current_time, 
            int next_wakeup_time, 
            PassengerQueue passengers_queue, 
            PassengerQueue serviced_queue,
            double mu,
            int next_arrival_time) {
        this.current_passenger = current_passenger;
        if (this.current_passenger != null) {
            System.out.println(current_passenger + " is initialized" );
        }
        this.current_time = current_time;
        this.next_wakeup_time = next_wakeup_time;
        this.passengers_queue = passengers_queue;
        this.serviced_queue = serviced_queue;
        this.mu = mu;
        this.next_arrival_time = next_arrival_time;
    }

    public void run() {
        if (this.current_time == this.next_wakeup_time) {

            if (this.current_passenger != null) {
                System.out.println("hurray");
                this.current_passenger.inspected_timestamp = this.current_time;
                this.current_passenger.state = Passenger.State.SERVICED;
                this.serviced_queue.addPassenger(this.current_passenger, Integer.MAX_VALUE);

                System.out.println(this.current_passenger.passenger_id + " is served");

                this.current_passenger = null;
            }

            Passenger new_passenger = this.passengers_queue.getPassenger();
            if (new_passenger != null) {
                new_passenger.state = Passenger.State.SERVICING;
                new_passenger.officer_timestamp = this.current_time;
                this.current_passenger = new_passenger;

                int service_time = Utility.getPoissonRandom(1.0/mu);
                this.next_wakeup_time += service_time;

                System.out.println(this.current_passenger.passenger_id + " is being served");
            }

            else {
                this.next_wakeup_time = next_arrival_time;
                System.out.println("No passenger");
                // System.out.println(this.current_passenger.passenger_id + " not served");
            }            
        }
    }
}