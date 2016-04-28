#from netCDF4 import Dataset
import sys
import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import matplotlib.animation as animation

if len(sys.argv)==1:
    fname = 'hgrad.dat'
else:
    fname = sys.argv[1]


data = np.genfromtxt(fname)
data2 = np.genfromtxt('hgrad_cadna.dat')
plt.clf()
plt.imshow(data-data2)
plt.colorbar()

names = fname.split('.')

plt.savefig('fig_' + names[0] + '.png')
plt.close("all")
#fig = plt.figure()


#data2 = np.genfromtxt('HfilA')
#1data2 = data2.reshape(-1,128,128)
ims=[]

def animate(i):
    z = data2[i,:,:]
    im = plt.imshow(np.tanh(z))
    im.set_clim([-1,1])
    plt.title(str(i))
    return(im,)

def init():
    plt.plot([],[])

#animate(0)
#plt.colorbar()
#ani = animation.FuncAnimation(fig, animate,frames = data2.shape[0])
#ani.save('hmovie.avi')
#plt.show()
