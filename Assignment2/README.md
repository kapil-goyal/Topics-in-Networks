# Instructions:
##  There is boolean variable which decides whether to take command line inputs or not.
  1. The default value of that boolean is true which means to take input from command line
  2. Input must be given in proper format and is case sensitive.
  3. To use hardcoded values, take input variable must be set to false and then simulation will run for different values depending on the code.

# How to compile
g++ app.cpp -o routing

# How to run
./routing −N 'switchportcount' −B 'buffersize' −p packetgenprob −queue 'INQ' | 'KOUQ' | 'ISLIP' −K 'knockout' −out 'outputfile' −T 'maxtimeslots'

e.g.: ./routing -N 7 -B 3 -p 0.6 -queue KOUQ -K 0.7 -out example.csv -T 500

Fill inputs without quotes and use proper integer and float formats.
Make sure to keep input consistencies (like if -N is specified, then next string should be integer).

If a value is not specified, default value will be used.

# Default values:
#### switch_port_count = 8;			
#### buffer_size = 4;				
#### packet_gen_prob = 0.5;			
#### queue_type = INQ;		
#### knockout = 0.6;				
#### output_file = "out.csv";		
#### max_time_slots = 10000;		