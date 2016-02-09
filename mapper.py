#!/usr/bin/env python

import sys
import os
import subprocess
# input comes from STDIN (standard input)
for line in sys.stdin:
    file_name = os.environ['mapreduce_map_input_file']
    # remove leading and trailing whitespace
    line = line.strip()
    # split the line into words
    words = line.split()
    # increase counters
command = "sudo /usr/local/hadoop/scripts/rec2 %s %s"%("/var/www/html/send/uploads/image.jpg","/usr/local/hadoop/scripts/1.yml")
proc1 = subprocess.Popen(command,stdout=subprocess.PIPE,shell=True)
(output1,error1) = proc1.communicate()
command = "sudo /usr/local/hadoop/scripts/rec2 %s %s"%("/var/www/html/send/uploads/image.jpg","/usr/local/hadoop/scripts/2.yml")
proc2 = subprocess.Popen(command,stdout=subprocess.PIPE,shell=True)
(output2,error2) = proc2.communicate()
command = "sudo /usr/local/hadoop/scripts/rec2 %s %s"%("/var/www/html/send/uploads/image.jpg","/usr/local/hadoop/scripts/3.yml")
proc3 = subprocess.Popen(command,stdout=subprocess.PIPE,shell=True)
(output3,error3) = proc3.communicate()
print '%s\t%s' % ("1",output1[0:output1.find(".")])
print '%s\t%s' % ("2",output2[0:output2.find(".")])
print '%s\t%s' % ("3",output3[0:output3.find(".")])
    #for word in words:
        # write the results to STDOUT (standard outpuet);
        # what we output here will be the input for the
        # Reduce step, i.e. the input for reducer.py
        #
        # tab-delimited; the trivial word count is 1
        

