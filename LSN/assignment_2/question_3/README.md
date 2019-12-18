# Analysing range capabilities

The directory contains two directories with scripts for the root node and leaf node, respectively called "ROOT" and "LEAF".

These scripts were run using the docker-environment installation of contiki-ng, so after installation you run the following commands:

# For the TSCH-only-stack

For the root instance:
* contiker
* cd LSN/assignment_1/question_2/energest-NON-6TiSCH/ROOT/
* make TARGET=zoul BOARD=firefly PORT=/dev/ttyUSB0 root.upload
* make TARGET=zoul BOARD=firefly PORT=/dev/ttyUSB0 login
* (it's recommended to reset the node here, so you don't miss out on output while logging in)
* tsch-set-coordinator 1

For the leaf instance:
* contiker
* cd LSN/assignment_1/question_2/energest-NON-6TiSCH/LEAF/
* make TARGET=zoul BOARD=firefly PORT=/dev/ttyUSB1 leaf.upload
* make TARGET=zoul BOARD=firefly PORT=/dev/ttyUSB1 login
* (it's recommended to reset the node here, so you don't miss out on output while logging in)


For the command:
make TARGET=zoul BOARD=firefly PORT=/dev/ttyUSB0 login
You could optionally replace this with:
make TARGET=zoul BOARD=firefly PORT=/dev/ttyUSB0 login | tee "name".txt
To get the output also written to a file


Explanation:
Unfortunatley this is were we ran into problems, we disabled the 6TiSCH stack like in the previous questions. Then we wanted to create a new link in the slotframe dedicated for only the leaf node. Creating this link worked, but the leaf node couldn't use it. Even adding a packet to the queue had issues and wouldn't build. 



# Parsing & Scripts

The scripts for postprocessing the experimental data is written in Python 3.0. Some additional libraries used are :
* pandas (https://pandas.pydata.org/)
To install pandas, use the following command : pip3 install pandas

There are two scripts that is essentially required to process and visualize the data. The basic  syntaxt of the scripts are:
* energy_consumption.py: **python3 energy_consumption.py directory/INPUTFILENAME.txt stats/OUTPUTFILENAME.csv**
* plot_consumption.py: **python3 plot_consumption.py stats/INPUTFILENAME.csv plots/OUTPUTFILENAME.png**


The parsing script essentially reads the energest log files and uses python 're' module to match some basic regular expressions. The two regex used are:
* To iterate through the energest dump file and extract the primary statistics, we have used this patters :'[ ]*CPU[ ]*(\d+)s[ ]*LPM[ ]*(\d+)s[ ]*DEEP LPM[ ]*(\d+)s[ ]*Total time[ ]*(\d+)s[ ]*[\n]*[ ]*Radio LISTEN[ ]*(\d+)s[ ]*TRANSMIT[ ]*(\d+)s[ ]*OFF[ ]*(\d+)s'


The chronology of commands to reproduce the statistics and plots is given below:

For computing the energy statistics:

```
python3 energy_consumption.py energest-6Tisch/LEAF/leaf.txt stats/6TISCH/leaf.csv 
python3 energy_consumption.py energest-6Tisch/LEAF/root.txt stats/6TISCH/root.csv 

```

For visualizing and generating the plot:

```
python3 plot_consumption.py stats/6TISCH/leaf.csv plots/6TISCH/leaf.png 
python3 plot_consumption.py stats/6TISCH/root.csv plots/6TISCH/root.png 
```
