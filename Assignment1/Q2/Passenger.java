import java.util.*;

class Passenger {

    enum State {
        WAITING,
        SERVICING,
        SERVICED
    }

    int passenger_id;
    int queue_timestamp;
    int officer_timestamp;
    int inspected_timestamp;
    State state;

    Passenger(int passenger_id, int queue_timestamp) {
        this.passenger_id = passenger_id;
        this.queue_timestamp = queue_timestamp;
        this.state = State.WAITING;
    }

    public int get_response_time() {
        if (this.state != State.SERVICED) {
            return -1;
        }
        else {
            return (this.inspected_timestamp - this.queue_timestamp);
        }
    }

    public int get_waiting_time() {
        if (this.state != State.WAITING) {
            return (this.officer_timestamp - this.queue_timestamp);
        }
        else {
            return -1;
        }
    }
}