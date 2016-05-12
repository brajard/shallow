#!/usr/bin/env python

exps = range(11)
fid = open('./doc/summary.tex','w')
for i in exps:
    ff = open('./output/summary_'+str(i)+'.log','r')
    fid.write(ff.readline())
    fid.write('\n\hline\n')
    ff.close
fid.close()
