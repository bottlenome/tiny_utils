#!/usr/bin/env python
import sys
import os
import subprocess

def getHash():
    p = subprocess.Popen("git rev-parse HEAD".split(" "), stdout=subprocess.PIPE)
    r, e = p.communicate()
    return r.strip()

def getNextHash(condition, index, hash_now):
    cmd = "grep " + condition + " 1 " + hash_now + " .hist"
    p = subprocess.Popen(cmd.split(" "), stdout=subprocess.PIPE)
    r, e = p.communicate()
    hashs = r.split('\n')
    if len(hashs) == 2:
        return ""
    else:
        return hashs[index]

def getUpHash(hash_now):
    return getNextHash("-B", 0, hash_now)

def getDownHash(hash_now):
    return getNextHash("-A", 1, hash_now)

def getOldestHash(hash_now):
    cmd = "tail -n 1 .hist"
    p = subprocess.Popen(cmd.split(" "), stdout=subprocess.PIPE)
    r, e = p.communicate()
    return r.strip()

def checkout(hash_next):
    os.system("git checkout " + hash_next)

def main(f):
    if not os.path.exists(".hist"):
        os.system('git log | egrep "^commit " | sed -e s/^commit\ //g > .hist')
        print("create hist")
    hash_now = getHash()
    hash_next = f(hash_now)
    checkout(hash_next)

if __name__ == '__main__':
    if sys.argv[1] == 'u':
        main(getUpHash)
    elif sys.argv[1] == 'd':
        main(getDownHash)
    elif sys.argv[1] == 'G':
        main(getOldestHash)
