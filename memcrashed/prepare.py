#!/usr/bin/env python
#Removes unspported lines from ipfile
import re
import sys
import time

def main():
    total_cnt=0
    unsupported_cnt=0
    if len(sys.argv)<2:
        print("\033[94m[*]:\033[0mUsage:%s <ipfile>"%sys.argv[0])
        sys.exit(-1)
pattern=re.compile('^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$')#ip regex
    ips=[]
    with open(sys.argv[1],"r") as f:
        for line in f:
            total_cnt+=1
            if pattern.match(line) is None:
                unsupported_cnt+=1
            else:
                ips.append(line)

    print("\033[92m[+]:\033[0mTotal lines:%d"%total_cnt)
    print("\033[92m[+]:\033[0mUnsupported lines:%d"%unsupported_cnt)
    print("\033[94m[*]:\033[0mRewritng file...")

    with open(sys.argv[1],"w+") as f:
        for ip in ips:
            f.write(ip)

if __name__=='__main__':
    start_time = time.time()
    try:
        main()
    except Exception as e:
        print("\033[91m[-]:\033[0mError while executing:%s"%unsupported_cnt)
        sys.exit(-1)
    elapsed_time = time.time() - start_time
    print("\033[92m[+]:\033[0mDone in %.2fs"%elapsed_time)
