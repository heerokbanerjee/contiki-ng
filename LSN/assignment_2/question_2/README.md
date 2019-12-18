# Analysing the TSCH joining process

We have created the following directory:
* energest-NON-6TiSCH: the files to run a TSCH-only stack

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
The root-node will start printing energy-stats each second over a period of 10 min and shuts itself down.
The leaf-node starts immediatley a timer. The moment it detects it has joined a network it will print out the time required to join and then disconnect to start over again. This way we can calculate a mean value for joining time. In between this output, the node will also print each second energy-stats. All this happens over a period of 10 min and then shuts itself down.



# Parsing & Scripts

The scripts for postprocessing the experimental data is written in Python 3.0. Some additional libraries used are :
* pandas (https://pandas.pydata.org/)
To install pandas, use the following command : pip3 install pandas

There are three scripts that is essentially required to process and visualize the data. The basic  syntaxt of the scripts are:

* energy_consumption.py: **python3 energy_consumption.py directory/INPUTFILENAME.txt stats/OUTPUTFILENAME.csv**

* joining_time.py: **python3 joining_time.py directory/INPUTFILENAME.txt stats/OUTPUTFILENAME.csv**

* mean_joining_time.py: **python3 mean_joining_time.py directoryname INPUTFILENAME.txt stats/OUTPUTFILENAME.csv**

In the last script, we can pass the string "all" instead of "INPUTFILENAME.txt" to compute the mean from all the recorded obeservations and output the mean joining time for each individual EB_period.


The parsing script essentially reads the energest log files and uses python 're' module to match some basic regular expressions. The two regex used are:
* To iterate through the energest dump file and extract the timing statistics, we have used this patters : **r'TIME NEEDED TO JOIN:[ ]*(\d+)'**


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
