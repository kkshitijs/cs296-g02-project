import os
import re

os.system('touch g02_data.csv') # move file to data folder
file1 = open("g02_data.csv" , "w") 
exp = re.compile(r"[0-9]+[.]*[0-9]*")

for j in range (1 , 101):
	for i in range (1, 101): #iteration value
		k = os.popen('./bin/cs296_exe {0}'.format(i)).read()
		k1 = exp.findall(k)
		#print(k1)
		file1.write("{0},{1},{2},{3},{4},{5},{6}\n".format(j,i,k1[1],k1[2],k1[3],k1[4],k1[5]))
file1.close()		
os.system('mv g02_data.csv ./data')		
