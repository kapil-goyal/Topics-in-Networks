#include <bits/stdc++.h>
using namespace std;

enum queue_type
{
	INQ, KOUQ, ISLIP
};

int current_time = 0;

struct Packet {

	int id;
	int input_queue_id;
	int output_queue_id;
	int arrival_time;
	int schedule_time;
	int completion_time;

	static int counter;

	Packet(int input_queue_id) {
		counter++;

		this->id = counter;
		this->input_queue_id = input_queue_id;
		this->arrival_time = current_time;
		this->output_queue_id = -1;
	}

	int getPacketDelay() {
		return this->completion_time - this->arrival_time;
	}

	void print() {
		printf("id: %d i/p: %d o/p: %d A: %d  S: %d C: %d|| ", id, input_queue_id, output_queue_id, arrival_time, schedule_time, completion_time);
	}
};

// boolean variable to decide wheter to take input from command line
bool to_take_input = false;

// Default value of parameters
int switch_port_count = 8;
int buffer_size = 4;
float packet_gen_prob = 1;
queue_type scheduler = ISLIP;
float knockout = 0.6;
string output_file = "out.csv";
int max_time_slots = 10000;

// Data structures used for simulation and statistics
int Packet::counter = 0;
int k_drop = 0;
list<Packet> dropped_packets;
list<Packet> transmitted_packets;
vector<list<Packet>> input_queues;
vector<list<Packet>> output_queues;
vector<int> accept_order;
vector<int> grant_order;

class PacketGenerator
{
public:
	bool toGenerate() {
		float p = (float) rand() / (float) RAND_MAX;
		return p <= packet_gen_prob;
	}

	int getOutputPort() {
		return rand() % switch_port_count;
	}

	void generate() {
		if (scheduler == ISLIP) {
			for (int i = 0; i < switch_port_count; ++i)
			{
				if (toGenerate()) {
					Packet new_packet(i);
					new_packet.output_queue_id = getOutputPort();
					int count = 0;
					for (int j = 0; j < switch_port_count; ++j)
					{	
						count += input_queues[i*switch_port_count + j].size();
					}
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

		for (int i = 0; i < input_queues.size(); ++i)
		{
			if (toGenerate()) {
				Packet new_packet(i);
				new_packet.output_queue_id = getOutputPort();
				if (input_queues[i].size() < buffer_size) {
					input_queues[i].push_back(new_packet);
				}
				else {
					dropped_packets.push_back(new_packet);
				}
			}
		}
	}

	void printInpQueues() {
		cout << "Input Queues:\n";
		for (auto& inp_queue : input_queues) {
			for (auto& packet : inp_queue) {
				packet.print();
			}cout << endl;
		}
	}
	
};

class Transmitter
{
public:

	void transmit() {
		for (int i = 0; i < output_queues.size(); i++) {
			if (output_queues[i].size() > 0) {
				Packet packet = output_queues[i].front();
				output_queues[i].pop_front();
				packet.completion_time = current_time+1;
				transmitted_packets.push_back(packet);
			}
		}
	}

	void printOutputQueues() {
		cout << "Output Queues:\n";
		for (auto& out_queue : output_queues) {
			for (auto& packet : out_queue) {
				packet.print();
			}cout << endl;
		}
	}
	
};

class Scheduler
{
	int k = knockout * switch_port_count;

public:
	vector<pair<int,int>> scheduleISLIP(vector<vector<int>> &request_table) {

		vector<pair<int, int>> final_mapping;
		while (1) {

			bool chk = 0;

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

			if (!chk) {
				break;
			}

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

		return final_mapping;

	}
	void schedule() {
		switch (scheduler)
		{
			case INQ:
			{
				map<int, vector<Packet>> port_mapper;
				for (auto& inp_queue : input_queues) {
					if (inp_queue.size() > 0) {
						auto packet = inp_queue.front();
						inp_queue.pop_front();
						port_mapper[packet.output_queue_id].push_back(packet);
					}
				}

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
			case KOUQ:
			{
				map<int, vector<Packet>> port_mapper;
				for (auto& inp_queue : input_queues) {
					if (inp_queue.size() > 0) {
						auto packet = inp_queue.front();
						inp_queue.pop_front();
						port_mapper[packet.output_queue_id].push_back(packet);
					}
				}

				for (auto it : port_mapper) {
					auto temp = it.second;
					for (int i = 0; i < k; i++) {
						if (temp.size() > 0) {
							int ind = rand() % temp.size();
							temp[ind].schedule_time = current_time;
							output_queues[it.first].push_back(temp[ind]);
							temp.erase(temp.begin() + ind);
						}
						else {
							break;
						}
					}
					if (temp.size() > 0) {
						k_drop++;
					}
				}
				break;
			}
			case ISLIP:
			{
				vector<vector<int>> request_table(switch_port_count, vector<int> (switch_port_count, 0));
				for (int i = 0; i < switch_port_count; i++) {
					for (int j = 0; j < switch_port_count; ++j)
					{
						if (input_queues[i*switch_port_count + j].size() > 0) {
							request_table[i][j] = 1;
						}
					}
				}

				vector<pair<int,int>> port_mapper = scheduleISLIP(request_table);
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

class stats_calculator
{
	string output_file;

public:
	stats_calculator(string output_file) {
		this->output_file =  output_file;
	}

	void printStats() {

		long long total_delay = 0;
		int total_packets = transmitted_packets.size();
		for (auto packet : transmitted_packets) {
			total_delay += packet.getPacketDelay();
		}
		double avg_PD = (double) total_delay / (double) total_packets;
		double avg_link_util = (double) total_packets / (double) (max_time_slots * switch_port_count);
		double variance_N = 0;
		for (auto packet : transmitted_packets) {
			variance_N += (packet.getPacketDelay() - avg_PD) * (packet.getPacketDelay() - avg_PD);
		}
		double std_dev_PD = sqrt(variance_N / total_packets);
		double avg_kouq_drop_prob = (double) k_drop / (double) (max_time_slots * switch_port_count);

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

void clearDB() {
	Packet::counter = 0;
	int k_drop = 0;	
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
		outfile << "Number of Ports" << "," << "Buffer Size" << "," << "Knockout" << "," << "Packet Gen Prob" << "," << "Queue Type" << "," << "Avg PD" 
				<< "," << "Std Dev PD" << "," << "Avg Link Utilization" << "," << "KOUQ Drop prob" << endl;
		outfile.close();
	}
}

void runSimulation() {
	Transmitter transmitter;
	Scheduler switch_fabric;
	PacketGenerator pg;
	clearDB();
	initializeOutputFile();
	for (current_time = 0; current_time < max_time_slots; ++current_time)
	{
		// pg.printInpQueues();
		// transmitter.printOutputQueues();

		transmitter.transmit();

		// pg.printInpQueues();
		// transmitter.printOutputQueues();

		switch_fabric.schedule();

		// pg.printInpQueues();
		// transmitter.printOutputQueues();

		pg.generate();

		// pg.printInpQueues();
		// transmitter.printOutputQueues();
	}

	stats_calculator sc(output_file);
	sc.printStats();
}


int main(int argc, char *argv[]) {

	srand(time(NULL));
	
	if (to_take_input) {

		for (int i = 0; i < argc; ++i)
		{
			cout << argv[i] << endl;
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
		runSimulation();
		return 0;
	}
	
	if (scheduler == INQ) {
		output_file = "INQ.csv";
	}
	else if (scheduler == KOUQ) {
		output_file = "KOUQ.csv";
	}
	else {
		output_file = "ISLIP.csv";
	}

	vector<int> ports({4,6,8,10,12,14,16});
	vector<int> buffer_sizes({2,3,4});
	vector<float> knockouts;


	if (scheduler == KOUQ) {
		knockouts.push_back(0.6);
		knockouts.push_back(0.8);
		knockouts.push_back(1.0);

		for (int i = 0; i < knockouts.size(); ++i)
		{
			knockout = knockouts[i];
			for (int j = 0; j < buffer_sizes.size(); ++j)
			{
				buffer_size = buffer_sizes[j];
				for (int k = 0; k < ports.size(); ++k)
				{
					switch_port_count = ports[k];
					runSimulation();					
				}
			}
		}
	}
	else {
		for (int i = 0; i < buffer_sizes.size(); ++i)
		{
			buffer_size = buffer_sizes[i];
			for (int j = 0; j < ports.size(); ++j)
			{
				switch_port_count = ports[j];
				runSimulation();
			}
		}
	}

	return 0;
}