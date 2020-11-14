#!/usr/bin/python

import random, sys
import argparse

class shuf:
    def __init__(self, lines):
        self.lines=lines

    def shufline(self, cnt=""):
        random.shuffle(self.lines)
        if cnt:
            for index in range(int(cnt)):
                print(self.lines[index])
        else:
            for index in range(len(self.lines)):
                 print(self.lines[index])

    def rshufline(self, cnt=""):
        if cnt:
            for index in range(int(cnt)):
                print(random.choice(self.lines))
        else:
            while True:
                print(random.choice(self.lines))

def main():
    parser = argparse.ArgumentParser(description= "Write a random permutation of the input lines to standard output.\n")
    
    parser.add_argument("-i", "--input-range", 
	action="store", dest = "irange", 
	help= "treat each number LO through HI as an input line")
    parser.add_argument("-n", "--head-count", action="store", 
	dest = "cnt", help= "output at most COUNT lines")
    parser.add_argument("-r", "--repeat", action="store_true", help=
        "output lines can be repeated")
    parser.add_argument("filename", nargs="*")
    args = parser.parse_args(sys.argv[1:])

    if args.irange:
        try:
            s = args.irange.split('-', 1)
            start = int(s[0])
            end = int(s[1])
            lines = list(range(start, end+1))
        except:
            parser.error("Error: Invalid input range")

    elif len(args.filename)==0 or args.filename[0] =='-':
        lines = sys.stdin.readlines()

    else:
        if len(args.filename) == 1:
            with open(args.filename[0]) as f:
                lines = f.read().splitlines()
        else:
            parser.error("Error: Too many operands")
   
    generator = shuf(lines)
    if args.repeat:
        generator.rshufline(args.cnt)
    else:
        generator.shufline(args.cnt)

if __name__ == "__main__":
    main()
