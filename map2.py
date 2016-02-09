import sys,subprocess,os,MySQLdb;

'''if(len(sys.argv)>2 & len(sys.argv)<1):
    print "help --> mapper.py image_name"
    sys.exit(1)
'''
command = "./face_detect %s"%("uploads/image.jpg")
proc1 = subprocess.Popen(command,stdout=subprocess.PIPE,shell=True)
(output,error) = proc1.communicate()

command = "./recognize %s"%("uploads/image.jpg")
proc2 = subprocess.Popen(command,stdout=subprocess.PIPE,shell=True)
(output2,error2) = proc2.communicate()

output2=output2.strip()
op2=output2.split("\n")
#print output2
image = op2[0]
im=int(image)
#image_name=image.split(":")
#get the name of the verified 
#print image_name[1]

# Open database connection
db = MySQLdb.connect("localhost","root","123","faces")

# prepare a cursor object using cursor() method
cursor = db.cursor()

sql = "select name,special_name from users where image_number=%d"%(im)
#print sql
cursor.execute(sql)

data = cursor.fetchone()
send_data = data[0]+","+data[1]
print sql
print send_data

db.close()
