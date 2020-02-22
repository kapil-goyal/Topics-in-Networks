import java.util.*;

class App {

    public static void main(String[] a) {

        Scanner input = new Scanner(System.in);
        System.out.print("Please enter duration of simulation in millisec: ");
        int simualation_time = input.nextInt();
        System.out.print("Please enter lambda in (passenger / ms): ");
        double lambda = input.nextDouble();
        System.out.print("Please enter mu in (passenger / ms): ");
        double mu = input.nextDouble();
        input.close();

        PassengerGenerator.lambda = lambda;

        int current_time = 0;
        int next_arrival_time = 0;
        int next_wakeup_time_officer1 = 0;
        int next_wakeup_time_officer2 = 0;
        int next_wakeup_time_officer3 = 0;
        Passenger current_passenger_1 = null;
        Passenger current_passenger_2 = null;
        Passenger current_passenger_3 = null;
        next_arrival_time += Utility.getPoissonRandom(1.0/lambda);
        PassengerGenerator.next_arrival_time = next_arrival_time;

        PassengerQueue queue1 = new PassengerQueue();
        PassengerQueue queue2 = new PassengerQueue();
        PassengerQueue queue3 = new PassengerQueue();
        PassengerQueue serviced_queue = new PassengerQueue();
        PassengerGenerator.queue1 = queue1;
        PassengerGenerator.queue2 = queue2;
        PassengerGenerator.queue3 = queue3;

        
        while (current_time <= simualation_time) {
            PassengerGenerator cg = new PassengerGenerator(current_time);
            Officer officer1 = new Officer (current_passenger_1,
                                            current_time, 
                                            next_wakeup_time_officer1, 
                                            queue1, 
                                            serviced_queue, 
                                            mu, 
                                            next_arrival_time);

            Officer officer2 = new Officer (current_passenger_2,
                                            current_time, 
                                            next_wakeup_time_officer2, 
                                            queue2, 
                                            serviced_queue, 
                                            mu, 
                                            next_arrival_time);

            Officer officer3 = new Officer (current_passenger_3,
                                            current_time, 
                                            next_wakeup_time_officer3, 
                                            queue3, 
                                            serviced_queue, 
                                            mu, 
                                            next_arrival_time);
                                    

            cg.start();
            officer1.start();
            officer2.start();
            officer3.start();
            try {
                cg.join();
                officer1.join();
                officer2.join();
                officer3.join();
            } catch (Exception e) {
                System.out.println(e);
                //TODO: handle exception
            }
            
            next_arrival_time = PassengerGenerator.next_arrival_time;
            next_wakeup_time_officer1 = officer1.next_wakeup_time;
            next_wakeup_time_officer2 = officer2.next_wakeup_time;
            next_wakeup_time_officer3 = officer3.next_wakeup_time;
            current_passenger_1 = officer1.current_passenger;
            current_passenger_2 = officer2.current_passenger;
            current_passenger_3 = officer3.current_passenger;
            if (current_passenger_1 != null) {
                System.out.println(current_passenger_1 + " sdfsfs");
            }
            if (current_passenger_2 != null) {
                System.out.println(current_passenger_2 + " sdfsfs");
            }
            if (current_passenger_3 != null) {
                System.out.println(current_passenger_3 + " sdfsfs");
            }
            current_time = next_arrival_time;
            current_time = current_time < officer1.next_wakeup_time ? current_time : officer1.next_wakeup_time;
            current_time = current_time < officer2.next_wakeup_time ? current_time : officer2.next_wakeup_time;
            current_time = current_time < officer3.next_wakeup_time ? current_time : officer3.next_wakeup_time;
        }

        queue1.printPassengers();
        queue2.printPassengers();
        queue3.printPassengers();
        serviced_queue.printPassengers();


        System.out.println(PassengerGenerator.passenger_id);
        System.out.println(queue1.getSize());
        System.out.println(queue2.getSize());
        System.out.println(queue3.getSize());
        System.out.println(serviced_queue.getSize());
        if (current_passenger_1 != null) {
            System.out.println(current_passenger_1 + " sdfsfs");
        }
        if (current_passenger_2 != null) {
            System.out.println(current_passenger_2 + " sdfsfs");
        }
        if (current_passenger_3 != null) {
            System.out.println(current_passenger_3 + " sdfsfs");
        }
    }
}