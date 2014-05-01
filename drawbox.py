from PIL import Image, ImageDraw
import sys

file = sys.argv[1]
num  = int(sys.argv[2])

f = open(file, 'r')
content = f.readlines()
f.close()

num_1 = int(content[0])
if num_1 == 0:
    print "No Solutions Were Found!"
    sys.exit()
if num_1 < num:
    num = num_1
if num < num_1:
    print str(num_1) + " solutions found, only showing " + str(num)
    
size = 50
buff = 100 
    
ints = content[1].split(' ')
width = int(ints[0])
height = int(ints[1])

for x in range(0,num):
    start = 2 + (height + 1)*x
    end = start + height
    values = [[0 for x in xrange(width)] for x in xrange(height)]
    for y in range(start,end):
        row = content[y].split(' ')
        for n in range(0,width):
            values[y-start][n] = int(row[n])
    im = Image.new('RGBA', (width * size + 2*buff, height * size + 2*buff), (255, 255, 255, 0))
    draw = ImageDraw.Draw(im) 
    for y in range(0,height):
        for n in range (0,width):
            value = int(values[y][n])
            if value != 0:
                if y == 0:
                    draw.line((n*size + buff,buff,(n+1)*size+buff,buff), fill=128)
                if y == height - 1:
                    draw.line((n*size + buff,(y+1)*size + buff,(n+1)*size + buff, (y+1)*size+buff), fill=128)
                    if n != width -1:
                        if value != values[y][n+1]:
                            draw.line(((n+1)*size+buff,y*size+buff,(n+1)*size+buff,(y+1)*size+buff), fill=128)
                if n == 0:
                    draw.line((buff,y*size + buff,buff,(y+1)*size+buff), fill=128)
                if n == width - 1:
                    draw.line(((n+1)*size + buff, y*size+buff,(n+1)*size+buff,(y+1)*size+buff), fill=128)
                    if y != height-1:
                        if value != values[y+1][n]:
                            draw.line((n*size+buff,(y+1)*size+buff,(n+1)*size+buff,(y+1)*size+buff),fill=128)
                if y != height -1:
                    if n != width-1:
                        if value != values[y+1][n]:
                            draw.line((n*size+buff,(y+1)*size+buff,(n+1)*size + buff, (y+1)*size+buff), fill=128)
                        if value != values[y][n+1]:
                            draw.line(((n+1)*size+buff,y*size+buff,(n+1)*size+buff,(y+1)*size+buff),fill=128)
                if y != 0:
                    if n != 0:
                        if value != values[y-1][n]:
                            draw.line((n*size+buff,y*size+buff,(n+1)*size+buff,y*size+buff),fill=128)
                        if value != values[y][n-1]:
                            draw.line((n*size+buff,y*size+buff,n*size+buff,(y+1)*size+buff),fill=128)
    im.show()
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    