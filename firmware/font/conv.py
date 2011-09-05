#!/usr/bin/python

ZOOM = 4		# one font pixel corresponds to 4x4 pixels in the image

import sys
import pygame
name = sys.argv[1]
img = pygame.image.load(name)

for char in range(32, 128):
	data = []
	for x in range(3):
		x = (char % 32 * 4 + x) * ZOOM
		bits = 0
		for y in range(5):
			y = (char / 32 * 6 + y) * ZOOM
			bits = bits * 2 + (img.get_at((x, y))[3] > 0)
		data.append("0x%02X" % bits)
	print "{%s}, // '%c'" % (",".join(data), char)

