#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Nov  9 18:40:21 2019

@author: heerokbanerjee
"""

mcolumns=['itr','CPU (in s)','LPM (in s)','DLPM (in s)',\
                                     'listen (in s)','Rx (in s)','Tx (in s)',\
                                     'CPU_Usage_profile (in J)','LPM_Usage_profile (in J)',\
                                     'DLPM_Usage_profile (in J)','Listen_usage_profile (in J)',\
                                     'Rx_usage_profile (in J)','Tx_usage_profile (in J)',\
                                     'TotalUsage_profile (in J)',\
                                     'CPU_Usage_datasheet (in J)','LPM_Usage_datasheet (in J)',\
                                     'DLPM_Usage_datasheet (in J)','Listen_usage_datasheet (in J)',\
                                     'Rx_usage_datasheet (in J)','Tx_usage_datasheet (in J)',\
                                     'TotalUsage_datasheet (in J)']

import pandas as pd
import re
import sys,os

try:
    test,inFilename,outFilename=sys.argv

except:
    print("Please specify both input and output filenames(with extension)!")
    exit(1)

myframe=pd.DataFrame(columns=mcolumns)
firstFrame=pd.DataFrame(data=[(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)],columns=mcolumns)
myframe=myframe.append(firstFrame)

pattern=r'[ ]*CPU[ ]*(\d+)s[ ]*LPM[ ]*(\d+)s[ ]*DEEP LPM[ ]*'\
        +r'(\d+)s[ ]*Total time[ ]*(\d+)s[ ]*[\n]'\
        +r'*[ ]*Radio LISTEN[ ]*(\d+)s[ ]*TRANSMIT[ ]*(\d+)s[ ]*OFF[ ]*(\d+)s'
file=open(inFilename,'r').read()
stats=re.findall(pattern,file)
#print(stats)
#Energy consumption parameters
inp_volt=3.3

### Device Profiling (in Amps)
CPU_profile_rate=15.35*pow(10,-3)
LPM_profile_rate=9.59*pow(10,-3)
DLPM_profile_rate=2.58*pow(10,-3)
listen_profile_rate=28.32*pow(10,-3)
Rx_profile_rate=30.14*pow(10,-3)
Tx_profile_rate=31.12*pow(10,-3)


### CC2538 datasheet (in Amps)
CPU_datasheet_rate=20*pow(10,-3)
LPM_datasheet_rate=0.6*pow(10,-3)
DLPM_datasheet_rate=0.0013*pow(10,-3)
listen_datasheet_rate=24*pow(10,-3)
Rx_datasheet_rate=27*pow(10,-3)
Tx_datasheet_rate=34*pow(10,-3)

### For each iteration
##
## We Take value at current iteration- value at prev iteration.
## This given the time utilized for consumption. Next, Using
## device profiling/ datasheet emperical values, we compute
## the energy consumption for that iteration.


counter=1
for values in stats:
    #print(values)
    #CPU consumption
    energy_CPU_profile=CPU_profile_rate*float(int(values[0])-int(myframe['CPU (in s)'].values[counter-1]))*inp_volt
    energy_CPU_datasheet=CPU_datasheet_rate*float(values[0])*inp_volt
    #LPM consumption
    energy_LPM_profile=LPM_profile_rate*float(int(values[1])-int(myframe['LPM (in s)'].values[counter-1]))*inp_volt
    energy_LPM_datasheet=LPM_datasheet_rate*float(values[1])*inp_volt
    #DeepLPM consumption
    energy_DLPM_profile=DLPM_profile_rate*float(int(values[2])-int(myframe['DLPM (in s)'].values[counter-1]))*inp_volt
    energy_DLPM_datasheet=DLPM_datasheet_rate*float(values[2])*inp_volt
    #Listen consumption
    energy_listen_profile=listen_profile_rate*float(int(values[3])-int(myframe['listen (in s)'].values[counter-1]))*inp_volt
    energy_listen_datasheet=listen_datasheet_rate*float(values[3])*inp_volt
    #Rx consumption
    energy_Rx_profile=Rx_profile_rate*float(int(values[4])-int(myframe['Rx (in s)'].values[counter-1]))*inp_volt
    energy_Rx_datasheet=Rx_datasheet_rate*float(values[4])*inp_volt
    #Tx consumption
    energy_Tx_profile=Tx_profile_rate*float(int(values[5])-int(myframe['Tx (in s)'].values[counter-1]))*inp_volt
    energy_Tx_datasheet=Tx_datasheet_rate*float(values[5])*inp_volt
    
    #Computing total energy consumption
    energy_total_profile=energy_CPU_profile+energy_LPM_profile+ \
                        energy_DLPM_profile+energy_listen_profile+ \
                        energy_Rx_profile+energy_Tx_profile
    energy_total_datasheet=energy_CPU_datasheet+energy_LPM_datasheet+ \
                      energy_DLPM_datasheet+energy_listen_datasheet+ \
                      energy_Rx_datasheet+energy_Tx_datasheet
    
        
    ## Create new Dataframe and append at each step.
    ## So that, in next iteration we can use the value.
    data=[[counter,values[0],values[1],values[2],values[3],values[4],values[5],\
          energy_CPU_profile,energy_LPM_profile,energy_DLPM_profile,\
          energy_listen_profile,energy_Rx_profile,energy_Tx_profile,\
          energy_total_profile,\
          energy_CPU_datasheet,energy_LPM_datasheet,energy_DLPM_datasheet,\
          energy_listen_datasheet,energy_Rx_datasheet,energy_Tx_datasheet,\
          energy_total_datasheet]]
    
    frame=pd.DataFrame(data,columns=mcolumns)
    
    myframe=myframe.append(frame)
    counter=counter+1

print(myframe)    

#Writing to csv File
mdir=outFilename.rsplit('/',1)
#print(mdir[0])
if not os.path.isdir(mdir[0]):
    os.makedirs(mdir[0]) 
export_to_csv=myframe.to_csv(outFilename,index=None,header=True)
