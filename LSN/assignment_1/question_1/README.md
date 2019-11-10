# Analysing the 6TiSCH energy consumption

We have created two different directories:
* energest-6Tisch: the files to run a entire 6TiSCH stack
* energest-NON-6TiSCH: the files to run a TSCH-only stack

Each directory contains two directories with scripts for the root node and leaf node, respectively called "ROOT" and "LEAF".

These scripts were run using the docker-environment installation of contiki-ng, so after installation you run the following commands:

# For the full 6TiSCH-stack

For the root instance:
* contiker
* cd LSN/assignment_1/question_1/energest-6Tisch/ROOT/
* make TARGET=zoul BOARD=firefly PORT=/dev/ttyUSB0 root.upload
* make TARGET=zoul BOARD=firefly PORT=/dev/ttyUSB0 login
* (it's recommended to reset the node here, so you don't miss out on output while logging in)
* tsch-set-coordinator 1
* rpl-set-root 1

For the leaf instance:
* contiker
* cd LSN/assignment_1/question_1/energest-6Tisch/LEAF/
* make TARGET=zoul BOARD=firefly PORT=/dev/ttyUSB1 leaf.upload
* make TARGET=zoul BOARD=firefly PORT=/dev/ttyUSB1 login
* (it's recommended to reset the node here, so you don't miss out on output while logging in)

Explanation:
Once the root-node detects it got neighbours, it will start printing energy-stats each 10 sec over a period of 10 min and shuts itself down.
Once the leaf-node detects it has joined a network, it will also print out energy-stats each 10 sec over a period of 10 min and then shut itself down.


# For the TSCH-only-stack

For the root instance:
* contiker
* cd LSN/assignment_1/question_1/energest-NON-6TiSCH/ROOT/
* make TARGET=zoul BOARD=firefly PORT=/dev/ttyUSB0 root.upload
* make TARGET=zoul BOARD=firefly PORT=/dev/ttyUSB0 login
* (it's recommended to reset the node here, so you don't miss out on output while logging in)
* tsch-set-coordinator 1

For the leaf instance:
* contiker
* cd LSN/assignment_1/question_1/energest-NON-6TiSCH/LEAF/
* make TARGET=zoul BOARD=firefly PORT=/dev/ttyUSB1 leaf.upload
* make TARGET=zoul BOARD=firefly PORT=/dev/ttyUSB1 login
* (it's recommended to reset the node here, so you don't miss out on output while logging in)


For the command:
make TARGET=zoul BOARD=firefly PORT=/dev/ttyUSB0 login
You could optionally replace this with:
make TARGET=zoul BOARD=firefly PORT=/dev/ttyUSB0 login | tee "name".txt
To get the output also written to a file


Explanation:
Once the root-node detects a node has joined the network, it will start printing energy-stats each 10 sec over a period of 10 min and shuts itself down.
Once the leaf-node detects it has joined a network it will also print out energy-stats each 10 sec over a period of 10 min and then shut itself down.

