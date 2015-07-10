straj
xivg dt 1.50e3
xivg dx 5000
xivg dy 5000
xivg pcor 0.0001
xivg grav 0.01
xivg dissip 0.00001
xivg alpha 0.15
xivg hmoy 100

xgauss 15 125000 12500 125000 12500
xvitgeo

#testdf 10 10 1 1 1 %0.000001 0.0001
#testdf 10 10 1 2 1 %0.000001 0.0001
#testdf 50 10 1 10 1 %0.000001 0.0001

set_modeltime 0
FORWARD
#xdisplay
#set_modeltime 0
#FORWARD
#xdisplay

set_iosep
#savestate Toce 1 ij 5% A 0 ./HfilT
#savestate Hfil 1 ij 5% A 0 ./HfilT
#savestate Ufil 1 ij 5% A 0 ./UfilT
#savestate Vfil 1 ij 5% A 0 ./VfilT
#savestate Hphy 1 ij 5% A 0 ./HphyT
#savestate Uphy 1 ij 5% A 0 ./UphyT
#savestate Vphy 1 ij 5% A 0 ./VphyT

#!gnuplot hsplot.gp
#goto fin
outoobs Hfil 1 SZA

xgauss 5 125000 12500 125000 12500
cost lms 0.5
#testof 0.02 10 10 8
print_cost ON

setm_impres 0
setm_io 6
setm_mode 0
#set_nbiter 100
set_nbiter 20
setm_nsim 200
setm_dxmin 1.0e-12
setm_epsg 1.0e-12
setm_ddf1 1.0

RUNM

savestate Hfil 1 ij 5% A 0 ./HfilA

#fin
