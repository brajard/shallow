#!/usr/bin/env python
import subprocess as sp
import os
import sys
import time
from mytools import find_res
lexp = []
lexp.append({'key':0,'label':'seq,ieee','opt':'','num_th':0})
lexp.append({'key':1,'label':'seq,cadna','opt':'--cadna','num_th':0})
lexp.append({'key':2,'label':'omp,ieee,atomic','opt':'--omp','num_th':1})
lexp.append({'key':3,'label':'omp,ieee,atomic,noreduc','opt':'--omp --noreduce','num_th':1})
lexp.append({'key':4,'label':'omp,ieee,atomic','opt':'--omp','num_th':16})
lexp.append({'key':5,'label':'omp,ieee,critical','opt':'--omp --critical','num_th':16})
lexp.append({'key':6,'label':'omp,cadna,critical','opt':'--omp --critical --cadna','num_th':16})
lexp.append({'key':7,'label':'omp,ieee,bug','opt':'--omp --error','num_th':16})
lexp.append({'key':8,'label':'omp,cadna,bug','opt':'--omp --error --cadna','num_th':16})
lexp.append({'key':9,'label':'omp,cadna,bug','opt':'--omp --error --cadna','num_th':32})
lexp.append({'key':10,'label':'omp,ieee,atomic','opt':'--omp','num_th':32})

tocompile=True
exptorun = [10]

for l in lexp:
    if l['key'] in exptorun:
        print 'run exp ', l['key'] , ' : ' , l['label'] , ' : ' , l['num_th'] , 'thds'
        if tocompile:
            print ' --> start compilation '
            start = time.time()
            log_compile=open('./output/compile_'+str(l['key'])+'.log','w')
            sp.call('./compile.py ' + l['opt'],shell=True,stderr=sp.STDOUT,stdout=log_compile)
            end = time.time()
            print '     completed in ',end-start, ' sec'
            log_compile.close()
            time.sleep(3)

        print ' --> run application '
        start = time.time()
        log_output=open('./output/run_'+str(l['key'])+'.log','w')
        cmdline = 'export OMP_NUM_THREADS='+str(l['num_th'])+';./shalw'
        output = sp.check_output(cmdline,shell=True,stderr=sp.STDOUT)
        log_output.write(output)
        end = time.time()
        log_output.close()
        print '     completed in ', end-start, ' sec'
        res = find_res('./output/run_'+str(l['key'])+'.log')
        print ' --> Res = ',res
        summary=open('./output/summary_'+str(l['key'])+'.log','w')
        summary.write (str(l['key'])+ ' & ')
        summary.write (l['label'] + ' & ')
        summary.write (str(l['num_th']) + ' & ')
        summary.write (res + ' \\\\')
        summary.close()
        


