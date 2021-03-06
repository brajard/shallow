defval SZX 128
defval SZY 128
 #defval SZX 1024
#defval SZY 1024
defval SZU 1
defval SZT 500
defval SZA 501

hat_name shalw 
  
//option o_gradtest
option o_parallel
//option o_cadna
//option o_m1qn3

exec disp_option

traj Toce M SZU SZT
//traj Toce2 M SZU 0 2 SZT

space Soce M SZX SZY Toce

modul Hfil space Soce input 3 output 1 tempo cout target
modul Ufil space Soce input 3 output 1 tempo
modul Vfil space Soce input 3 output 1 tempo
modul Hphy space Soce input 5 output 1 tempo
modul Uphy space Soce input 7 output 1 tempo
modul Vphy space Soce input 7 output 1 tempo

ctin Hfil 1 from Hfil 1 i   j   t-1
ctin Hfil 2 from Hphy 1 i   j   t-1
ctin Hfil 3 from Hphy 1 i   j   t 

ctin Vfil 1 from Vfil 1 i   j   t-1
ctin Vfil 2 from Vphy 1 i   j   t-1
ctin Vfil 3 from Vphy 1 i   j   t 

ctin Ufil 1 from Ufil 1 i   j   t-1
ctin Ufil 2 from Uphy 1 i   j   t-1
ctin Ufil 3 from Uphy 1 i   j   t 

ctin Hphy 1 from Hfil 1 i   j   t-1
ctin Hphy 2 from Uphy 1 i   j   t-1
ctin Hphy 3 from Uphy 1 i-1 j   t-1
ctin Hphy 4 from Vphy 1 i   j+1 t-1
ctin Hphy 5 from Vphy 1 i   j   t-1

ctin Vphy 1 from Vfil 1 i   j   t-1
ctin Vphy 2 from Hphy 1 i   j   t-1
ctin Vphy 3 from Hphy 1 i   j-1 t-1
ctin Vphy 4 from Uphy 1 i-1 j-1 t-1
ctin Vphy 5 from Uphy 1 i-1 j   t-1
ctin Vphy 6 from Uphy 1 i   j-1 t-1
ctin Vphy 7 from Uphy 1 i   j   t-1

ctin Uphy 1 from Ufil 1 i   j   t-1
ctin Uphy 2 from Hphy 1 i+1 j   t-1
ctin Uphy 3 from Hphy 1 i   j   t-1
ctin Uphy 4 from Vphy 1 i   j   t-1
ctin Uphy 5 from Vphy 1 i   j+1 t-1
ctin Uphy 6 from Vphy 1 i+1 j   t-1
ctin Uphy 7 from Vphy 1 i+1 j+1 t-1

order modinspace Soce
  order YA1 YA2
        Hphy 
	Uphy 
	Vphy
	Hfil 
	Ufil 
	Vfil
  forder
forder

order spaceintraj Toce
  Soce
forder

insert_fct arg xivg
insert_fct     xdisplay
insert_fct arg xgauss
insert_fct     xvitgeo
insert_fct arg savegrad
insert_fct     adjoint
insert_fct     print_normgrad
insert_fct     compute_res