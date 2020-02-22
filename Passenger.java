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
}