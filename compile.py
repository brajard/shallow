#!/usr/bin/env python
import subprocess as sp
import os
import sys
import getopt

rootdir = '..'
target = 'shallow'
o_error = False ##to comment atomic/critical pragma
omp = False ##to compile in omp
cadna  = False ## to add cadna
critical = False ## with atomic id false (or critical if true)

try:
    opts,args = getopt.getopt(sys.argv[1:],"",["rootdir=","target=","error","omp","cadna","critical"])
except getopt.GetoptError:
    print "Wrong parameters"
    sys.exit(2)

for opt,arg in opts:
    if opt in ("--rootdir"):
        rootdir = arg
    elif opt in ("--target"):
        target = arg
    elif opt in ("--error"):
        o_error = True
    elif opt in ("--omp"):
        omp = True
    elif opt in ("--cadna"):
        cadna = True
    elif opt in ("--critical"):
        critical = True

opt_yao =' -e '

#Some Warnings
warnmess=''
if not omp and critical:
    warnmess += 'Critical option has no effect if omp is inactive\n'
if not omp and o_error:
    warnmess += 'Error option has no effect if omp is inactive\n'
if omp and cadna and not critical:
    warnmess += 'parallel cadna code with atomic should fail to compile\n'

#Set YAO options
if omp:
    sedcmd = "sed -i 's/\/\/option o_parallel/option o_parallel/g' shalw.d"
    opt_yao += ' -p '
else:
    sedcmd = "sed -i 's/^option o_parallel/\/\/option o_parallel/g' shalw.d"
sp.call(sedcmd,shell=True)

if cadna:
    sedcmd = "sed -i 's/\/\/option o_cadna/option o_cadna/g' shalw.d"
    opt_yao += ' -cadna '
    if omp:
        cadnaCdir='$cadnaComp'
    else:
        cadnaCdir='$cadnaCseq'
                
else:
    sedcmd = "sed -i 's/^option o_cadna/\/\/option o_cadna/g' shalw.d"
 

sp.call(sedcmd,shell=True)

#First compilation (can be wrong) to generate the code
gencmd=''
if cadna:
    gencmd = "export cadnaCdir="+cadnaCdir + ";"
gencmd +="yao92 " + opt_yao + "shalw"
    
print gencmd
sp.call(gencmd,shell=True)

recompile= False
#Some modifications in the generated code in some cases
if omp and critical and not o_error:
    recompile = True
    sedcmd = "sed -i 's/#pragma omp atomic/#pragma omp critical/g' Yworkdir/"
    sp.call(sedcmd+'TrajectoryToce.hpp',shell=True)
    sp.call(sedcmd+'Y2shalw.h',shell=True)

if omp and o_error:
    recompile = True
    sedcmd = "sed -i 's/#pragma omp atomic/\/\/#pragma omp atomic/g' Yworkdir/"
    sp.call(sedcmd+'TrajectoryToce.hpp',shell=True)
    sp.call(sedcmd+'Y2shalw.h',shell=True)
   
if omp and cadna:
    recompile = True
    sedcmd = "sed -i '/^double_st[ \t]*Yjac/ s/;/={0};/g' Yworkdir/Y1shalw.h"
#    sp.call(sedcmd,shell=True)

print 'recompile=',recompile

#Second compilation (sould be correct)
if recompile:
    gencmd +="yao92 " + opt_yao + "-c shalw"
    print gencmd
    sp.call(gencmd,shell=True)

#print options
print 'omp=',omp
print 'cadna=',cadna
print 'o_error=',o_error


#Warnings:
if len(warnmess)==0:
    print '\nCOMPILATION WITHOUT WARNINGS'
else:
    print'\nWARNINGS DURING THE COMPILATION:\n'
    print warnmess
