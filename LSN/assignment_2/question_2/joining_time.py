#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Dec 17 22:40:12 2019

@author: heerokbanerjee
"""

import sys,os,re
import pandas as pd

def get_join_times(inFile):
    pattern2=r'TIME NEEDED TO JOIN:[ ]*(\d+)'
    file=open(inFile,'r').read()
    join_times=re.findall(pattern2,file)
    return join_times

try:
    test,inFilename,outFilename=sys.argv

except:
    print("Please specify both input and output filenames!")
    exit(1)

#file=open(inFilename,'r')
eb_data=get_join_times(inFilename)
#print(eb_data)
myframe=pd.DataFrame()
count=1

for eb_value in eb_data:

	data=[[count,eb_value]]
	mframe=pd.DataFrame(data,columns=['itr','JoiningTime'])
	myframe=myframe.append(mframe)
	count=count+1

#Writing to csv File
mdir=outFilename.rsplit('/',1)
if not os.path.isdir(mdir[0]):
	os.makedirs(mdir[0]) 
	print("directory created")
        
export_to_csv=myframe.to_csv(outFilename,index=None,header=True)

