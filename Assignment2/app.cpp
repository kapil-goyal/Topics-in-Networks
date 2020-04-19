#include <bits/stdc++.h>
using namespace std;

// Enum to store queue type 
enum queue_type
{
	INQ, KOUQ, ISLIP
};

// This variable will store current time slot
int current_time = 0;

// boolean variable to decide wheter to take input from command line
bool to_take_input = true;


// Struct to make packet
struct Packet {

	int id;					// Id of packet
	int input_queue_id;		// id of input queue it is generated in
	int output_queue_id;	// id of output queue it is destined for
	int arrival_time;		// time slot at which packet arrives
	int schedule_time;		// time stot at which packet is scheduled
	int completion_time;	// time slot at which packet is transmitted

	static int counter;		// static variable to generate packet id

	// constructor function
	Packet(int input_queue_id) {
		counter++;

		this->id = counter;
		this->input_queue_id = input_queue_id;
		this->arrival_time = current_time;
		this->output_queue_id = -1;
	}

	// This function will return packet delay
	int getPacketDelay() {
		return this->completion_time - this->arrival_time;
	}

	// A utility function to print packet details
	void print() {
		printf("id: %d i/p: %d o/p: %d A: %d  S: %d C: %d|| ", id, input_queue_id, output_queue_id, arrival_time, schedule_time, completion_time);
	}
};


// Default value of parameters
int switch_port_count = 8;			// Nummber of input and output ports
int buffer_size = 4;				// Buffer size
float packet_gen_prob = 0.5;			// packet generation probability at input ports
queue_type scheduler = INQ;		// queue type for each case
float knockout = 0.6;				// knockout factor in case of KOUQ scheduler
string output_file = "out.csv";		// default output file name
int max_time_slots = 10000;			// default maximum number of slots

// Data structures used for simulation and statistics
int Packet::counter = 0;			// Initilization of packet id generator counter
int k_drop = 0;						// A variable to count KOUQ dropout probability
list<Packet> dropped_packets;		// A list to store dropped packets
list<Packet> transmitted_packets;	// A list to store transmitter packets
vector<list<Packet>> input_queues;	// Input Queues
vector<list<Packet>> output_queues;	// Output Queues
vector<int> accept_order;			// Accept priority order in case of ISLIP
vector<int> grant_order;			// Grant priority order in case of ISLIP


// A class to take care of packet generation in all the cases
class PacketGenerator
{
public:
	// A function to decide packet generation according to packet generation probability
	bool toGenerate() {
		float p = (float) rand() / (float) RAND_MAX;
		return p <= packet_gen_prob;
	}

	// A function to get output port randomly
	int getOutputPort() {
		return rand() % switch_port_count;
	}

	// A function to generate packets in input queue according to queue type
	void generate() {
		// Case of ISLIP
		if (scheduler == ISLIP) {
			// for each input queue
			for (int i = 0; i < switch_port_count; ++i)
			{
				// If packet is to be generated
				if (toGenerate()) {
					// generate a new packet
					Packet new_packet(i);
					// get output port of the queue.
					new_packet.output_queue_id = getOutputPort();

					// A variable to count total number of packets in the input queue
					int count = 0;
					for (int j = 0; j < switch_port_count; ++j)
					{	
						count += input_queues[i*switch_port_count + j].size();
					}

					// if number of packets in the input queue is less than buffer size
					if (count < buffer_size) {
						input_queues[i*switch_port_count + new_packet.output_queue_id].push_back(new_packet);
					}
					else {
						dropped_packets.push_back(new_packet);
					}
				}
			}
			return;
		}

		// Case of KOUQ of INQ
		// for each input queue
		for (int i = 0; i < input_queues.size(); ++i)
		{
			// If packet is to be generated
			if (toGenerate()) {
				// Generate a new packet
				Packet new_packet(i);
				new_packet.output_queue_id = getOutputPort();

				// If input queue size is less than buffer size
				if (input_queues[i].size() < buffer_size) {
					input_queues[i].push_back(new_packet);
				}
				else {
					dropped_packets.push_back(new_packet);
				}
			}
		}
	}

	// A utility function to print input queues.
	void printInpQueues() {
		cout << "Input Queues:\n";
		for (auto& inp_queue : input_queues) {
			for (auto& packet : inp_queue) {
				packet.print();
			}cout << endl;
		}
	}
	
};

// A class to take care of transmission
class Transmitter
{
public:

	// A function to transmit
	void transmit() {
		// for all output queues
		for (int i = 0; i < output_queues.size(); i++) {
			// if output queue is not empty transmit the packet
			if (output_queues[i].size() > 0) {
				// extract the front packet
				Packet packet = output_queues[i].front();
				output_queues[i].pop_front();

				// Set its transmission completion time
				packet.completion_time = current_time+1;

				// push it to the list of transmitted packets
				transmitted_packets.push_back(packet);
			}
		}
	}

	// A utility function to print output queues
	void printOutputQueues() {
		cout << "Output Queues:\n";
		for (auto& out_queue : output_queues) {
			for (auto& packet : out_queue) {
				packet.print();
			}cout << endl;
		}
	}
	
};

// A class to take care of scheduling
class Scheduler
{
	// The integral value of knockout in case of KOUQ
	int k = knockout * switch_port_count;

public:

	// This function completes a single round of islip based on request table
	vector<pair<int,int>> scheduleISLIP(vector<vector<int>> &request_table) {

		// A variable to store final matching after 1 round of iSLIP
		vector<pair<int, int>> final_mapping;
		while (1) {

			// A variable to stop the round.
			bool chk = 0;

			// A for loop to construct auxiliary table for grant phase
			vector<vector<int>> grant_table_aux(switch_port_count, vector<int> (switch_port_count, 0));
			for (int i = 0; i < switch_port_count; ++i)
			{
				for (int j = 0; j < switch_port_count; ++j)
				{
					if (request_table[i][j] == 1) {
						grant_table_aux[j][i] = 1;
						chk = 1;
					}
				}
			}

			// If request table is empty then break the loop
			if (!chk) {
				break;
			}

			// A for loop to construct auxiliary table for request phase
			vector<vector<int>> accept_table_aux(switch_port_count, vector<int> (switch_port_count, 0));
			for (int i = 0; i < switch_port_count; ++i)
			{
				for (int j = 0; j < switch_port_count; ++j)
				{
					if (grant_table_aux[i][(j + grant_order[i]) % switch_port_count] == 1) {
						accept_table_aux[(j + grant_order[i]) % switch_port_count][i] = 1;
						break;
					}
				}
			}
			
			// Construct final mapping using auxiliary accept table
			for (int i = 0; i < switch_port_count; ++i)
			{
				for (int j = 0; j < switch_port_count; ++j)
				{
					if (accept_table_aux[i][(j + accept_order[i]) % switch_port_count] == 1) {
						final_mapping.push_back({i, (j + accept_order[i]) % switch_port_count});
						break;
					}
				}
			}

			// Using final mapping, modify request table for next iteration
			for (auto it : final_mapping) {
				accept_order[it.first] = (it.second + 1) % switch_port_count;
				grant_order[it.second] = (it.first + 1) % switch_port_count;
				for (int i = 0; i < switch_port_count; ++i)
				{
					request_table[it.first][i] = 0;
					request_table[i][it.second] = 0;
				}
			}
		}

		// return the final matching for a round of iSLIP
		return final_mapping;

	}

	// A function to schedule packets for each case.
	void schedule() {
		switch (scheduler)
		{
			// In case of INQ
			case INQ:
			{
				// A map from output port to input ports
				map<int, vector<Packet>> port_mapper;

				// for each input queue check front packet and map it to output port
				for (auto& inp_queue : input_queues) {
					if (inp_queue.size() > 0) {
						auto packet = inp_queue.front();
						inp_queue.pop_front();
						port_mapper[packet.output_queue_id].push_back(packet);
					}
				}

				//  for each output port, select one packet randomly
				for (auto it : port_mapper) {
					auto temp = it.second;
					if (temp.size() > 0) {
						int ind = rand() % temp.size();
						temp[ind].schedule_time = current_time;
						output_queues[it.first].push_back(temp[ind]);
						for (int i = 0; i < temp.size(); i++) {
							if (i != ind) {
								input_queues[temp[i].input_queue_id].push_front(temp[i]);
							}
						}
					}
				}
				break;
			}

			// In case of KOUQ
			case KOUQ:
			{
				// A map from output port to input ports
				map<int, vector<Packet>> port_mapper;

				// for each input port select front packet and map it to output port
				for (auto& inp_queue : input_queues) {
					if (inp_queue.size() > 0) {
						auto packet = inp_queue.front();
						inp_queue.pop_front();
						port_mapper[packet.output_queue_id].push_back(packet);
					}
				}

				// for each output port
				for (auto it : port_mapper) {
					auto temp = it.second;

					// if packets are more than k, then increase kouq dropout count
					if (temp.size() > k) {
						k_drop++;
					}

					// select k packets randomly and push them to output queue
					for (int i = 0; i < k; i++) {
						if (temp.size() > 0) {
							int ind = rand() % temp.size();
							temp[ind].schedule_time = current_time;
							if (output_queues[it.first].size() < buffer_size)
								output_queues[it.first].push_back(temp[ind]);
							else
								dropped_packets.push_back(temp[ind]);
							temp.erase(temp.begin() + ind);
						}
						else {
							break;
						}
					}
				}
				break;
			}

			// In case of ISLIP
			case ISLIP:
			{
				// First make request table for each input port
				vector<vector<int>> request_table(switch_port_count, vector<int> (switch_port_count, 0));
				for (int i = 0; i < switch_port_count; i++) {
					for (int j = 0; j < switch_port_count; ++j)
					{
						if (input_queues[i*switch_port_count + j].size() > 0) {
							request_table[i][j] = 1;
						}
					}
				}

				// get final mapping from schedule islip function
				vector<pair<int,int>> port_mapper = scheduleISLIP(request_table);

				// push packets according to final mapping.
				for (auto it : port_mapper) {
					Packet packet = input_queues[it.first * switch_port_count + it.second].front();
					input_queues[it.first * switch_port_count + it.second].pop_front();
					packet.schedule_time = current_time;
					output_queues[it.second].push_back(packet);
				}
				break;
			}
		}
	}	
};


// A class to take care of statistics generation
class stats_calculator
{
	// output file to output statistics
	string output_file;

public:
	stats_calculator(string output_file) {
		this->output_file =  output_file;
	}

	// This function will calculate stats and will output them in output file
	void printStats() {

		// get total delay for all transmitted packets
		long long total_delay = 0;
		int total_packets = transmitted_packets.size();
		for (auto packet : transmitted_packets) {
			total_delay += packet.getPacketDelay();
		}

		// get average packet delay
		double avg_PD = (double) total_delay / (double) total_packets;

		// get average link utilization
		double avg_link_util = (double) total_packets / (double) (max_time_slots * switch_port_count);

		// Calculate variance
		double variance_N = 0;
		for (auto packet : transmitted_packets) {
			variance_N += (packet.getPacketDelay() - avg_PD) * (packet.getPacketDelay() - avg_PD);
		}

		// Calculate standard deviation for packet delay
		double std_dev_PD = sqrt(variance_N / total_packets);

		// Calculate average packet drop probability
		double avg_kouq_drop_prob = (double) k_drop / (double) (max_time_slots * switch_port_count);


		// Print statistics to output for each case
		if (scheduler == KOUQ) {
			ofstream outfile;
			outfile.open(output_file, ios_base::app);
			outfile << switch_port_count << "," << buffer_size << "," << knockout << "," << packet_gen_prob << "," << "KOUQ" << "," << avg_PD 
					<< "," << std_dev_PD << "," << avg_link_util << "," << avg_kouq_drop_prob << endl;
			outfile.close();
		}
		else if (scheduler == INQ){
			ofstream outfile;
			outfile.open(output_file, ios_base::app);
			outfile << switch_port_count << "," << buffer_size << "," << "NA" << "," << packet_gen_prob << "," << "INQ" << "," << avg_PD 
					<< "," << std_dev_PD << "," << avg_link_util << "," << "NA" << endl;
			outfile.close();
		}
		else {
			ofstream outfile;
			outfile.open(output_file, ios_base::app);
			outfile << switch_port_count << "," << buffer_size << "," << "NA" << "," << packet_gen_prob << "," << "ISLIP" << "," << avg_PD 
					<< "," << std_dev_PD << "," << avg_link_util << "," << "NA" << endl;
			outfile.close();
		}
	}	
};

// a function to set default values
void set_defaults() {
	// Default value of parameters
	switch_port_count = 8;			// Nummber of input and output ports
	buffer_size = 4;				// Buffer size
	packet_gen_prob = 0.5;			// packet generation probability at input ports
	knockout = 0.6;				// knockout factor in case of KOUQ scheduler
	output_file = "out.csv";		// default output file name
	max_time_slots = 10000;			// default maximum number of slots
}

// A function to clear data structures used, for the next iteration
void clearDB() {
	Packet::counter = 0;
	k_drop = 0;	
	input_queues.clear();
	output_queues.clear();
	accept_order.clear();
	grant_order.clear();
	dropped_packets.clear();
	transmitted_packets.clear();
	if (scheduler == ISLIP) {
		input_queues.resize(switch_port_count * switch_port_count);
		accept_order.resize(switch_port_count, 0);
		grant_order.resize(switch_port_count, 0);
	}
	else {
		input_queues.resize(switch_port_count);
	}
	output_queues.resize(switch_port_count);
}


// A function to generate header of output file
void initializeOutputFile() {
	int count = 0;
	string line;
    ifstream file(output_file);
	if(file.is_open())
	{
	    while(!file.eof())
		{
	        getline(file,line);
	        // cout<< line << endl;
	        count++;
	    }
	    file.close();
	}
	count--;
	if (count <= 0) {
		ofstream outfile;
		outfile.open(output_file, ios_base::app);
		outfile << "Number of Ports" << "," << "Buffer Size" << "," << "Knockout" << "," << "Packet Generation Probability" << "," << "Queue Type" << "," << "Average Packet Delay (in slots)" 
				<< "," << "Standard Deviation PD (in slots)" << "," << "Average Link Utilization" << "," << "KOUQ Drop probability" << endl;
		outfile.close();
	}
}

// A function which will run simulation
void runSimulation() {

	// Initialize transmitter, scheduler and packet generator
	Transmitter transmitter;
	Scheduler switch_fabric;
	PacketGenerator pg;

	// Clear all the data structures
	clearDB();

	// Initilize output file
	initializeOutputFile();

	// for each time slot
	for (current_time = 0; current_time < max_time_slots; ++current_time)
	{
		// pg.printInpQueues();
		// transmitter.printOutputQueues();

		// transmit
		transmitter.transmit();

		// pg.printInpQueues();
		// transmitter.printOutputQueues();

		// schedule
		switch_fabric.schedule();

		// pg.printInpQueues();
		// transmitter.printOutputQueues();

		// generate
		pg.generate();

		// pg.printInpQueues();
		// transmitter.printOutputQueues();
	}

	// print the statistics to output file
	stats_calculator sc(output_file);
	sc.printStats();
}


int main(int argc, char *argv[]) {

	srand(time(NULL));
	
	// If we want to take command line input
	if (to_take_input) {

		// Take inputs
		for (int i = 0; i < argc; ++i)
		{
			// cout << argv[i] << endl;
			string str(argv[i]);
			if (str == "-N") {
				switch_port_count = atoi(argv[i+1]);
			}
			if (str == "-B") {
				buffer_size = atoi(argv[i+1]);
			}
			if (str == "-p") {
				packet_gen_prob = atof(argv[i+1]);
			}
			if (str == "-queue") {
				string str2(argv[i+1]);
				if (str2 == "INQ") {
					scheduler = INQ;
				}
				else if (str2 == "KOUQ") {
					scheduler = KOUQ;
				}
				else if (str2 == "ISLIP") {
					scheduler = ISLIP;
				}
			}
			if (str == "-K") {
				knockout = atof(argv[i+1]);
			}
			if (str == "-out") {
				output_file = (string) argv[i+1];
			}
			if (str == "-T") {
				max_time_slots = atoi(argv[i+1]);
			}
		}
		// Run the simulation
		runSimulation();
		return 0;
	}
	
	// If we don't want to take command line inputs

	// initialize different output ports, buffer sizes and knockouts value
	vector<int> ports;
	for (size_t i = 4; i < 51; i++)
	{
		ports.push_back(i);
	}
	vector<int> buffer_sizes({2,3,4});
	vector<float> knockouts({0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0});
	vector<float> probs({0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0});
	vector<queue_type> schedulers({INQ, KOUQ, ISLIP});

	// Run simulation to compare three scheduling techniques
	for (auto sch : schedulers) {
		set_defaults();
		scheduler = sch;
		string temp;
		if (scheduler == INQ) {
			temp = "INQ";
		}
		else if (scheduler == KOUQ) {
			temp = "KOUQ";
		}
		else {
			temp = "ISLIP";
		}

		// For different number of ports
		for (auto it : ports) {
			switch_port_count = it;
			output_file = "ports_out.csv";
			runSimulation();
		}

		// For different number of packet generation probability
		for (auto it : probs) {
			packet_gen_prob = it;
			output_file = "probs_out.csv";
			runSimulation();
		}

		// For different buffer sizes
		for (auto it : buffer_sizes) {
			buffer_size = it;
			output_file = "buffer_size_out.csv";
			runSimulation();
		}
	}

	// For different knockout values in case of KOUQ
	set_defaults();
	scheduler = KOUQ;
	for (auto it : knockouts) {
		knockout = it;
		output_file = "kouq_k_out.csv";
		runSimulation();
	}


	// Run simulation in case of KOUQ
	set_defaults();
	scheduler = KOUQ;
	vector<float> knockouts2({0.6, 0.8, 1.0});
	for (int i = 0; i < knockouts2.size(); ++i)
	{
		knockout = knockouts2[i];
		for (int k = 0; k < ports.size(); ++k)
		{
			switch_port_count = ports[k];
			output_file = "kouq_k_n_out.csv";
			runSimulation();					
		}
	}

	set_defaults();
	for (int i = 0; i < buffer_sizes.size(); ++i)
	{
		buffer_size = buffer_sizes[i];
		for (int k = 0; k < ports.size(); ++k)
		{
			switch_port_count = ports[k];
			output_file = "kouq_b_n_out.csv";
			runSimulation();					
		}
	}

	set_defaults();
	scheduler = ISLIP;
	// Run simulation in case of ISLIP
	for (int i = 0; i < buffer_sizes.size(); ++i)
	{
		buffer_size = buffer_sizes[i];
		for (int j = 0; j < ports.size(); ++j)
		{
			switch_port_count = ports[j];
			output_file = "islip_b_out.csv";
			runSimulation();
		}
	}

	set_defaults();
	scheduler = INQ;
	// Run simulation in case of INQ
	for (int i = 0; i < buffer_sizes.size(); ++i)
	{
		buffer_size = buffer_sizes[i];
		for (int j = 0; j < ports.size(); ++j)
		{
			switch_port_count = ports[j];
			output_file = "inq_b_out.csv";
			runSimulation();
		}
	}

	return 0;
}