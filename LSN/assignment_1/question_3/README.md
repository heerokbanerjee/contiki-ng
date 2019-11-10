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
