straj
xivg dt 1.50e3
xivg dx 5000
xivg dy 5000
xivg pcor 0.0001
xivg grav 0.01
xivg dissip 0.00001
xivg alpha 0.15
xivg hmoy 100

#xgauss 15 125000 12500 125000 12500
xgauss 15
xvitgeo

set_modeltime 0
FORWARD
xdisplay


set_iosep


outoobs Hfil 1 SZA

#xgauss 5 125000 12500 125000 12500
xgauss 10
cost lms 0.5
print_cost ON

setepsi_all 0
set_nbiter 1
adjoint


#savestate Hfil 1 ij 5% A 0 ./HfilA
savegrad hgrad.dat
#fin
