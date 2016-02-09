import os,sys,subprocess,MySQLdb;

command="sudo /usr/local/hadoop/bin/hadoop jar /usr/local/hadoop/share/hadoop/tools/lib/hadoop-*streaming*.jar -mapper /usr/local/hadoop/scripts/mapper.py -reducer /usr/local/hadoop/scripts/reducer.py -input /user/root/yml/4.yml -output output"
proc1 = subprocess.Popen(command,stdout=subprocess.PIPE,shell=True)
(output1,error1) = proc1.communicate()

command="sudo /usr/local/hadoop/bin/hadoop fs -cat /user/root/output/*"
proc2 = subprocess.Popen(command,stdout=subprocess.PIPE,shell=True)
(output2,error2) = proc2.communicate()


command = "sudo /usr/local/hadoop/bin/hadoop fs -rm -r -f /user/root/output"
proc3 = subprocess.Popen(command,stdout=subprocess.PIPE,shell=True)
(output3,error3) = proc3.communicate()
val=[]
lines=output2.strip()
lines=lines.split("\n")
for line in lines:
	line=line.split("\t")
	val+=line

#odd goes for file_name and even goes for eigen values
if int(val[1]) < int(val[3]) :
	if int(val[1]) < int(val[5]):
		least = "1"
	else:
		least ="3"
else:
	if int(val[3]) < int(val[5]):
		least = "2"
	else:
		least = "3"

	
db = MySQLdb.connect("localhost","root","123","faces")
cursor=db.cursor()
sql="select name,special_name from users where image_number=%s"%(least)
cursor.execute(sql)
data=cursor.fetchone()
send_data=data[0]+","+data[1]
db.close()
print send_data
