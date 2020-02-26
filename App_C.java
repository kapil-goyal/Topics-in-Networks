import java.util.*;

class App_C {

    public static void main(String[] a) {

        Scanner input = new Scanner(System.in);
        System.out.print("Please enter duration of simulation in hour: ");
        int simualation_time = input.nextInt();
        System.out.print("Please enter lambda in (passenger / hour): ");
        double lambda = input.nextDouble();
        System.out.print("Please enter mu in (passenger / hour): ");
        double mu = input.nextDouble();
        input.close();

        if (lambda / 3 >= mu) {
            System.out.println("Error: Lambda is greater than mu, please try again");
            return;
        }

        PassengerGenerator.lambda = lambda;
        PassengerGenerator.MAX_SIZE = 10;

        int current_time = 0;
        int next_arrival_time = 0;
        int next_wakeup_time_officer1 = 0;
        int next_wakeup_time_officer2 = 0;
        int next_wakeup_time_officer3 = 0;
        Passenger current_passenger_1 = null;
        Passenger current_passenger_2 = null;
        Passenger current_passenger_3 = null;
        next_arrival_time += Utility.getNextTime(lambda);
        PassengerGenerator.next_arrival_time = next_arrival_time;

        PassengerQueue queue1 = new PassengerQueue();
        PassengerQueue queue2 = new PassengerQueue();
        PassengerQueue queue3 = new PassengerQueue();
        PassengerQueue serviced_queue = new PassengerQueue();
        PassengerGenerator.queue1 = queue1;
        PassengerGenerator.queue2 = queue2;
        PassengerGenerator.queue3 = queue3;

        int[] count_serviced = new int [simualation_time+1];
        int[] count_waiting = new int [simualation_time+1];
        int[] count_getting_inspected = new int [simualation_time+1];
        
        for (int i = 0; i < simualation_time+1; i++) {
            count_getting_inspected[i] = 0;
            count_waiting[i] = 0;
            count_serviced[i] = 0;
        }

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
            int count = 0;
            if (current_passenger_1 != null) {
                // System.out.println(current_passenger_1 + " sdfsfs");
                count++;
            }
            if (current_passenger_2 != null) {
                // System.out.println(current_passenger_2 + " sdfsfs");
                count++;
            }
            if (current_passenger_3 != null) {
                // System.out.println(current_passenger_3 + " sdfsfs");
                count++;
            }

            count_serviced[current_time] = serviced_queue.getSize();
            count_waiting[current_time] = queue1.getSize() + queue2.getSize() + queue3.getSize();
            count_getting_inspected[current_time] = count;

            int previous_time = current_time;
            current_time = next_arrival_time;
            current_time = current_time < officer1.next_wakeup_time ? current_time : officer1.next_wakeup_time;
            current_time = current_time < officer2.next_wakeup_time ? current_time : officer2.next_wakeup_time;
            current_time = current_time < officer3.next_wakeup_time ? current_time : officer3.next_wakeup_time;
            current_time = current_time < previous_time + 1 ? current_time : previous_time + 1;
        }

        // queue1.printPassengers();
        // queue2.printPassengers();
        // queue3.printPassengers();
        // serviced_queue.printPassengers();

        // System.out.println(PassengerGenerator.passenger_id);
        // System.out.println(queue1.getSize());
        // System.out.println(queue2.getSize());
        // System.out.println(queue3.getSize());
        // System.out.println(serviced_queue.getSize());
        if (current_passenger_1 != null) {
            // System.out.println(current_passenger_1 + " sdfsfs");
        }
        if (current_passenger_2 != null) {
            // System.out.println(current_passenger_2 + " sdfsfs");
        }
        if (current_passenger_3 != null) {
            // System.out.println(current_passenger_3 + " sdfsfs");
        }

        int total_serviced = 0;
        int total_waited = 0;
        int total_service_time = 0;
        int total_waited_time = 0;

        for (Passenger passenger : serviced_queue.passengers) {
            total_waited++;
            total_waited_time += passenger.get_waiting_time();

            total_serviced++;
            total_service_time += passenger.get_response_time();
        }
        if (current_passenger_1 != null) {
            total_waited++;
            total_waited_time += current_passenger_1.get_waiting_time();
        }
        if (current_passenger_2 != null) {
            total_waited++;
            total_waited_time += current_passenger_2.get_waiting_time();
        }
        if (current_passenger_3 != null) {
            total_waited++;
            total_waited_time += current_passenger_3.get_waiting_time();
        }

        int total_getting_inspected = 0;
        int total_waiting = 0;
        int total_count_serviced = 0;
        for (int itr: count_getting_inspected) {
            total_getting_inspected += itr;
        }
        for (int itr: count_serviced) {
            total_count_serviced += itr;
        }
        for (int itr: count_waiting) {
            total_waiting += itr;
        }

        System.out.println("\nSimulation Results:");
        // System.out.println("Total passenger generated: " + PassengerGenerator.passenger_id);
        // System.out.println("Avg passenger generated: " + ((float)PassengerGenerator.passenger_id / (float)simualation_time));
        // System.out.println("Total passengers inspected: " + total_serviced);
        // System.out.println("Avg passengers inspected: " + ((float)total_serviced / (float)simualation_time));
        // System.out.println("Total passengers getting inspected: " + total_getting_inspected);
        System.out.println("Avg number of passengers getting inspected: " + (float)total_getting_inspected / (float)simualation_time);
        // System.out.println("Total response time: " + total_service_time);
        System.out.println("Avg response time: " + ((float)total_service_time/(float)total_serviced));
        // System.out.println("Total waiting time: " + total_waited_time);
        System.out.println("Avg waiting time: " + ((float)total_waited_time/(float)total_waited));
        // System.out.println("Total passengers inspected: " + total_count_serviced);
        // System.out.println("Avg passengers inspected: " + (float)total_count_serviced / (float)simualation_time);
        // System.out.println("Total passengers waiting: " + total_waiting);
        System.out.println("Avg number of passengers waiting in the queue: " + (float)total_waiting / (float)simualation_time);


        double rho = lambda / (3 * mu);
        double prob_b = (1 - rho) * Math.pow(rho, 10.0) / (1 - Math.pow(rho, 11));

        double avg_passenger_system = 3*(rho / (1-rho) - (11) * Math.pow(rho, 11) / (1 - Math.pow(rho, 11)));
        double avg_response_time = avg_passenger_system / (lambda * (1 - prob_b));
        double avg_waiting_time = avg_response_time - 1 / mu;
        double avg_waiting_passengers = avg_waiting_time * (lambda) * (1 - prob_b);
        
        double avg_passenger_getting_inspected = avg_passenger_system - avg_waiting_passengers;

        System.out.println("\nTheoretical Results:");
        // System.out.println("Avg number of passengers in the system: " + avg_passenger_system);        
        System.out.println("Avg number of passengers getting inspected: " + avg_passenger_getting_inspected);
        System.out.println("Avg response time: " + avg_response_time);
        System.out.println("Avg waiting time: " + avg_waiting_time);
        System.out.println("Avg number of passengers waiting in the queue: " + avg_waiting_passengers);

    }
}