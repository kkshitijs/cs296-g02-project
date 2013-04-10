############################################################
#################imports####################################
import numpy as np
from math import sqrt
############################################################
filename = "./data/g02_data.csv"
output1 = './plots/g02_project_plot00.png'
output2 = './plots/g02_project_plot01.png'
output3 = './plots/g02_project_plot02.png'
output4 = './plots/g02_project_plot03.png'
output5 = './plots/g02_project_plot04.png'
output6 = './plots/g02_project_plot05.png'


step_avg_run_iter = np.loadtxt(filename,delimiter=',',usecols=(1,2))
loop_avg_run_iter = np.loadtxt(filename,delimiter=',',usecols=(1,6))
col_avg_run_iter = np.loadtxt(filename,delimiter=',',usecols=(1,3))
vel_avg_run_iter = np.loadtxt(filename,delimiter=',',usecols=(1,4))
pos_avg_run_iter = np.loadtxt(filename,delimiter=',',usecols=(1,5))
standard_dev = []
deleted_index=[]
for x in range(100):
	sstep = 0
	sloop = 0
	scol = 0
	svel = 0
	spos = 0
	avgsqr = 0
	for y in range(100):
		sstep  = sstep + step_avg_run_iter[x+(y*100)][1]
		sloop  = sloop + loop_avg_run_iter[x+(y*100)][1]
		scol  = scol + col_avg_run_iter[x+(y*100)][1]
		svel  = svel + vel_avg_run_iter[x+(y*100)][1]
		spos  = spos + pos_avg_run_iter[x+(y*100)][1]
		avgsqr = avgsqr +(step_avg_run_iter[x+(y*100)][1]**2)
		if y != 0:
			deleted_index.append(x+(y*100))
	step_avg_run_iter[x][1] = sstep/100
	loop_avg_run_iter[x][1] = sloop/100
	col_avg_run_iter[x][1] = scol/100
	vel_avg_run_iter[x][1] = svel/100
	pos_avg_run_iter[x][1] = spos/100
	standard_dev.append(sqrt((avgsqr/100)-(sstep**2/10000)))

step_avg_run_iter = np.delete(step_avg_run_iter,deleted_index,axis=0)
loop_avg_run_iter = np.delete(loop_avg_run_iter,deleted_index,axis=0)
col_avg_run_iter = np.delete(col_avg_run_iter,deleted_index,axis=0)
vel_avg_run_iter = np.delete(vel_avg_run_iter,deleted_index,axis=0)
pos_avg_run_iter = np.delete(pos_avg_run_iter,deleted_index,axis=0)


#####################################################################

step_avg_iter_run = np.loadtxt(filename,delimiter=',',usecols=(0,2))
loop_avg_iter_run = np.loadtxt(filename,delimiter=',',usecols=(0,6))
col_avg_iter_run = np.loadtxt(filename,delimiter=',',usecols=(0,3))
vel_avg_iter_run = np.loadtxt(filename,delimiter=',',usecols=(0,4))
pos_avg_iter_run = np.loadtxt(filename,delimiter=',',usecols=(0,5))

deleted_index=[]
for x in range(100):
	sstep = 0
	sloop = 0
	scol = 0
	svel = 0
	spos = 0
	for y in range(100):
		sstep  = sstep + step_avg_iter_run[x*100+y][1]
		sloop  = sloop + loop_avg_iter_run[x*100+y][1]
		scol  = scol + col_avg_iter_run[x*100+y][1]
		svel  = svel + vel_avg_iter_run[x*100+y][1]
		spos  = spos + pos_avg_iter_run[x*100+y][1]
		if y != 0:
			deleted_index.append(x*100+y)
	step_avg_iter_run[x*100][1] = sstep/100
	loop_avg_iter_run[x*100][1] = sloop/100
	col_avg_iter_run[x*100][1] = scol/100
	vel_avg_iter_run[x*100][1] = svel/100
	pos_avg_iter_run[x*100][1] = spos/100

step_avg_iter_run = np.delete(step_avg_iter_run,deleted_index,axis=0)
loop_avg_iter_run = np.delete(loop_avg_iter_run,deleted_index,axis=0)
col_avg_iter_run = np.delete(col_avg_iter_run,deleted_index,axis=0)
vel_avg_iter_run = np.delete(vel_avg_iter_run,deleted_index,axis=0)
pos_avg_iter_run = np.delete(pos_avg_iter_run,deleted_index,axis=0)

################################################################################
var_time_reruns = np.loadtxt(filename,delimiter=',',usecols=(0,2))
deleted_index = []
vartime = np.zeros((100,2),dtype=np.float)
for x in range(100):
	l = []
	for y in range(100):
		l.append(var_time_reruns[x*100+y][1])
	l = sorted(l)
	k = l[-1] - l[1]
	vartime[x][0]= x+1
	vartime[x][1] = k
		

### Lowest roll numner 1110050016

###iteration number 16

iter_step_var = np.loadtxt(filename,delimiter=',',usecols=(0,2))
deleted_index = []
for x in range(100):
	for y in range(100):
		if y != 15:
			deleted_index.append(x*100+y)

iter_step_var = np.delete(iter_step_var,deleted_index,axis=0)
##################################################################################

###########PLOT 1#################################################################
import matplotlib.pyplot as plt
import pylab
from matplotlib.pyplot import *
from numpy import *
p1,= plt.plot(step_avg_run_iter[:,0],step_avg_run_iter[:,1],'ro',label="average step time over reruns for iteration values")
p2,=plt.plot(loop_avg_run_iter[:,0],loop_avg_run_iter[:,1],'bo',label="average loop time over reruns for iteration values")
plt.ylabel("average time over reruns")
plt.xlabel("iteration values")
legend([p1,p2],["avg step_t over reruns for itervalues","avg loop_t over reruns for itervalues"])
pylab.savefig(output1)
plt.clf()
####################################################################################################
p1, = plt.plot(step_avg_run_iter[:,0],step_avg_run_iter[:,1],'ro',label="avg step_t over reruns for itervalues")
p2, = plt.plot(col_avg_run_iter[:,0],col_avg_run_iter[:,1],'bo',label="avg col_t over reruns for itervalues")
p3, = plt.plot(vel_avg_run_iter[:,0],vel_avg_run_iter[:,1],'yo',label="avg vel_t over reruns for itervalues")
p4, = plt.plot(pos_avg_run_iter[:,0],pos_avg_run_iter[:,1],'go',label="avg pos_t over reruns for itervalues")

plt.xlabel("avg profile variables over reruns")
plt.ylabel("iter values")
legend([p1,p2,p3,p4],["avg step_t over reruns for itervalues","avg col_t over reruns for itervalues",
	    "avg vel_t over reruns for itervalues","avg pos_t over reruns for itervalues"])
pylab.savefig(output2)
plt.clf()
########################################################################################################
p1,= plt.plot(step_avg_iter_run[:,0],step_avg_iter_run[:,1],'ro',label="average step time over iters for rerun values")
p2,=plt.plot(loop_avg_iter_run[:,0],loop_avg_iter_run[:,1],'bo',label="average loop time over iters for rerun values")
plt.ylabel("average time over iters")
plt.xlabel("rerun values")
legend([p1,p2],["avg step_t over iters for rerunvalues","avg loop_t over iters for rerunvalues"])
pylab.savefig(output3)
plt.clf()
###########################################################################################################
p1, = plt.plot(step_avg_iter_run[:,0],step_avg_iter_run[:,1],'ro',label="avg step_t over iters for rerunvalues")
p2, = plt.plot(col_avg_iter_run[:,0],col_avg_iter_run[:,1],'bo',label="avg col_t over iters for rerunvalues")
p3, = plt.plot(vel_avg_iter_run[:,0],vel_avg_iter_run[:,1],'yo',label="avg vel_t over iters for rerunvalues")
p4, = plt.plot(pos_avg_iter_run[:,0],pos_avg_iter_run[:,1],'go',label="avg pos_t over iters for rerunvalues")

plt.xlabel("avg profile variables over iters")
plt.ylabel("iter values")
legend([p1,p2,p3,p4],["avg step_t over iters for rerunvalues","avg col_t over iters for rerunvalues",
	    "avg vel_t over iters for rerunvalues","avg pos_t over iters for rerunvalues"])
pylab.savefig(output4)
plt.clf()
############################################################################################################
plt.figure()
p = plt.errorbar(step_avg_run_iter[:,0],step_avg_run_iter[:,1],yerr=standard_dev,fmt='o')
plt.xlabel("iter values")
plt.ylabel("step time for var itervalues")
legend([p],["step_t for various iter values with error bars"])
pylab.savefig(output5)
#############################################################################################################
plt.figure()
hst,bin= np.histogram(iter_step_var[:,1])
binsize = sorted(bin)[1] - sorted(bin)[0]
hst1 = []
hst1 = hst + hst
for i in range(len(hst1)):
	if i > 0:
		hst1[i] = hst1[i] + hst1[i-1]

p= plt.bar(bin[:-1],height=hst,width=binsize,color='crimson',label="step_t variation with  no_reruns as freq")
p1 = plt.plot(bin[:-1],hst1/2,'b',label="cummulative frequency")
plt.xlabel("avg step time ")
plt.ylabel("frequency")
legend()
pylab.savefig(output6)
