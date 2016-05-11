straj
#xivg dt 1.50e3
#xivg dx 5000
#xivg dy 5000
#xivg pcor 0.0001
#xivg grav 0.01
#xivg dissip 0.00001
#xivg alpha 0.15
#xivg hmoy 100
xivg dt 1800
xivg dx 20000
xivg dy 20000

xivg pcor 3.5e-5
xivg grav 0.02
xivg dissip 2e-7
xivg alpha 0.025
xivg hmoy 500


#xgauss 15 125000 12500 125000 12500
xgauss 15
#xvitgeo

set_modeltime 0
FORWARD
xdisplay


set_iosep


outoobs Hfil 1 SZA

#xgauss 5 125000 12500 125000 12500
xgauss 10
cost lms 0.5
print_cost OFF

setepsi_all 0
set_nbiter 1

#testof 0.02 10 10 8 0
#testof 2e-6 10 10 1 0
compute_res
goto fin
adjoint


#savestate Hfil 1 ij 5% A 0 ./HfilA
savegrad hgrad.dat
print_normgrad
goto fin
fin
