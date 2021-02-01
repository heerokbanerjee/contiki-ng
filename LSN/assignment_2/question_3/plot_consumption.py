#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Nov 10 19:14:44 2019

@author: heerokbanerjee
"""

import pandas as pd
import matplotlib.pyplot as plt
import sys,os

try:
    test,inFilename,outFilename=sys.argv

except:
    print("Please specify both input and output filenames(with extension)!")
    exit(1)

fig=plt.gcf()    
data=pd.read_csv(inFilename)
curr_axis=plt.gca()

data.plot(kind='line',x='itr',y='CPU_Usage_profile (in J)',ax=curr_axis)
data.plot(kind='line',x='itr',y='LPM_Usage_profile (in J)',ax=curr_axis)
data.plot(kind='line',x='itr',y='DLPM_Usage_profile (in J)',ax=curr_axis)
data.plot(kind='line',x='itr',y='Listen_usage_profile (in J)',ax=curr_axis)
data.plot(kind='line',x='itr',y='Rx_usage_profile (in J)',ax=curr_axis)
data.plot(kind='line',x='itr',y='Tx_usage_profile (in J)',ax=curr_axis)
data.plot(kind='line',x='itr',y='TotalUsage_profile (in J)',ax=curr_axis)

#plt.show()


#Saving Figure as png
  
#Writing to csv File
mdir=outFilename.rsplit('/',1)
#print(mdir[0])
if not os.path.isdir(mdir[0]):
    os.makedirs(mdir[0]) 
    
fig.savefig(outFilename)