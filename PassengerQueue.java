import java.util.*;
import java.util.concurrent.locks.ReentrantLock;

class PassengerQueue {

    Queue<Passenger> passengers;
    private final ReentrantLock lock;

    PassengerQueue() {
        passengers = new LinkedList<Passenger>();
        lock = new ReentrantLock();
    }

    public Passenger getPassenger() {
        lock.lock();
        if (passengers.size() > 0) {
            Passenger new_passenger = passengers.remove();
            lock.unlock();
            return new_passenger;
        }
        else {
            lock.unlock();
            return null;
        }
    }

    public boolean addPassenger(Passenger new_passenger, int max_size) {
        lock.lock();
        if (this.getSize() < max_size) {
            passengers.add(new_passenger);
            lock.unlock();
            return true;
        } else {
            lock.unlock();
            return false;
        }
    }

    public boolean isEmpty() {
        return (passengers.size() == 0);
    }

    public int getSize() {
        lock.lock();
        int result = passengers.size();
        lock.unlock();
        return result;
    }

    public void printPassengers() {
        System.out.println(passengers);
    }
}