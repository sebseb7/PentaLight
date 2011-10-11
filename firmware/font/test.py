#!/usr/bin/python
import sys
for l in sys.stdin:
	l = l.strip()
	a = map(eval,[ l[1:5], l[6:10], l[11:15]])
	print l[-2]
	x = "\n".join(map("".join,zip(*["%05d"%int(bin(x)[2:]) for x in a])))
	print x.replace("0", "  ").replace("1", "\x1b[42m  \x1b[49m")
