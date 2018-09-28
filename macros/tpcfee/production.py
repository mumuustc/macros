#!/usr/bin/env python3

from subprocess import Popen, PIPE, STDOUT
import argparse
import sys
import os
import subprocess
# import os.system
from sys import stdout

parser = argparse.ArgumentParser(description='Run bunch of jobs')
parser.add_argument('Dir', metavar='Dir_Name',
                   help='Directory name')
args = parser.parse_args()

print("Processing folder ", args.Dir)

subprocess.run(["env"]);
subprocess.run('pwd');
subprocess.run(['ls','-lhvc'])

for root, dirs, files in os.walk(args.Dir):
    for file in files:
        if file.endswith(".evt"):
            file = os.path.join(root, file)
            
            fout = open(file + ".out", "w");
            ferr = open(file + ".err", "w");
            
            print("processing ", file, " and output to " , file + ".out")
            
            subprocess.run(["nice" , "root" , "-q" , "-b" , "Fun4All_TPCFEE.C(30000000, \"" + file + "\")'"],
                            check=True , stdout=fout, stderr=ferr)  #
             
            fout.close()
            ferr.close()
            
#             logfile = open(file + ".log", "w");
#             proc = subprocess.Popen(["nice" , "root" , "-l" , "-b" , "'Fun4All_TPCFEE(30000000, \"" + file + "\")'"]
#                                   , stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
#             for line in proc.stdout:
#                 sys.stdout.write(line)
#                 logfile.write(line)
#             proc.wait()
#             
#             logfile.close()
            
#             with Popen(["nice" , "root" , "-l" , "-b" , "'Fun4All_TPCFEE(30000000, \"" + file + "\")'"],
#                         stdout=PIPE, stderr=STDOUT, bufsize=1) as p, \
#                 open(file + ".log", 'ab') as logfile:
#                     for line in p.stdout:  # b'\n'-separated lines
#                         sys.stdout.buffer.write(line)  # pass bytes as is
#                         logfile.write(line)
#                 
#             logfile.close()

            print("Completed processing ", file)
