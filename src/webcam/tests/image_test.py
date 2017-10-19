#!/usr/bin/python

import Image, ImageDraw

W = 640 #640
H = 480 #480

img = Image.new("RGB", (W, H), "black")
draw = ImageDraw.Draw(img)

f = open("sortie", "r")
f = f.read()

pixels = f.split(":")
x = y = 0
for p in pixels:
	(r,g,b) = p.split(",")
	color = (int(r), int(g), int(b))
        draw.point((x,y), fill=color)
	if (x+1 != 640):
		x += 1
	else:
		x = 0
		y += 1

img.save("img.png", "PNG")
