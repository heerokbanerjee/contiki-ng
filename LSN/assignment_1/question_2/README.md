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
