#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Nov 10 19:40:12 2019

@author: heerokbanerjee
"""

import pandas as pd
import re
import sys,os,subprocess

  
def get_join_times(inFile):
    pattern2=r'TIME NEEDED TO JOIN:[ ]*(\d+)'
    file=open(inFile,'r').read()
    join_times=re.findall(pattern2,file)
    return join_times

def get_mean(mlist):
    summ=0
    for element in mlist:
        summ=summ+int(element)
        
    mean=summ/len(mlist)
    return mean

try:
    test,inDir,inFilename,outFilename=sys.argv

except:
    print("Please specify Directory, input and output filenames(with extension)!")
    exit(1)


myframe=pd.DataFrame()
if not os.path.isdir(inDir):
    print("No such input Directory exists!")
    exit(1)

curr_pwd=os.getcwd() 
os.chdir(inDir)
if inFilename=="all":
    #os.system("pwd")
    out = subprocess.Popen(['ls | grep _EB_'], 
           stdout=subprocess.PIPE,shell=True, 
           stderr=subprocess.STDOUT)
    stdout,sterr=out.communicate()
    stdout=str(stdout)
    #print(stdout)
    
    pattern1=r'((leaf|root)_EB_\d+\.txt)'
    eb_files=re.findall(pattern1,stdout)
    #eb_files=eb_files.remove('leaf')
    #print(eb_files)
    for file in eb_files:
        #print(file)
        EBs=file[0].rsplit('.txt')
        eb=EBs[0].rsplit('_')
        #print(eb)
        curr_eb=eb[2]
        #print(curr_eb)
        
        curr_eb_join_time=get_join_times(str(file[0]))
        #print(curr_eb_join_time)
        mean_join_time=get_mean(curr_eb_join_time)
        #print(mean_join_time)
        
        data=[[curr_eb,mean_join_time]]
        mframe=pd.DataFrame(data,columns=['EB_period','Joining Time (Mean)'])
        myframe=myframe.append(mframe)
        
    print(myframe)
    os.chdir(curr_pwd)
    
    #Writing to csv File
    mdir=outFilename.rsplit('/',1)
    if not os.path.isdir(mdir[0]):
        os.makedirs(mdir[0]) 
        print("directory created")
        
    export_to_csv=myframe.to_csv(outFilename,index=None,header=True)
    
    

else:
    join_times=get_join_times(inFilename)
    ## Code for individual TSCH session
    ###
    ### To do
    print(join_times)
  