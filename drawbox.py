from PIL import Image, ImageDraw            # Import the proper libraries, PIL, Image draw to draw image
import sys                                  # sys to read command line arguments

file = sys.argv[1]                  # The file which contains the solutions data, assumed to be well formed
num  = int(sys.argv[2])             # The maximum number of solutions to display, if provided
if num > 20:
    num = 20

f = open(file, 'r')                 # open the file in read mode
content = f.readlines()             # Read all the lines into a list called 'content'
f.close()                           # close the file

num_1 = int(content[0])             # The first line in the file specifies how many solutions were found
if num_1 == 0:                      # If no solutions were found, alert the user and exit
    print "No Solutions Were Found!"
    sys.exit()
if num_1 < num:                     # If there were fewer found solutions than allowed by user
    num = num_1                     # Set variable equal to the number of solutions found
if num < num_1:                     # If more solutions were found, alert the user
    print str(num_1) + " solutions found, only showing " + str(num)
    
size = 50                           # Variable size of a block which makes up a piece
buff = 100                          # Variable buffer on top, bottom, left, and right of image
    
ints = content[1].split(' ')        # Split the second line of the file with delim ' '
width = int(ints[0])                # The width of the solution in blocks
height = int(ints[1])               # The height of the solution in blocks

for x in range(0,num):              # Begin loop, loop through all solutions ($num of them)
    start = 2 + (height + 1)*x      # Gives the start line in the file for this solution
    end = start + height            # Gives the end line in the file for this solution
    values = [[0 for x in xrange(width)] for x in xrange(height)]   # A 2D array that will hold the solution
    for y in range(start,end):                                      # Loop through the lines
        row = content[y].split(' ')
        for n in range(0,width):
            values[y-start][n] = int(row[n])                        # Initialize the array
    im = Image.new('RGBA', (width * size + 2*buff, height * size + 2*buff), (255, 255, 255, 0))
    draw = ImageDraw.Draw(im)                                       # Configure new image
    for y in range(0,height):               
        for n in range (0,width):
            value = int(values[y][n])                       
            if value != 0:                  # Below is the algorithm to draw lines
                if y == 0:                  # Just a bunch of if conditionals...
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
                if n == 0:
                    if y != 0:
                        if value != values[y-1][n]:
                            draw.line((n*size+buff,y*size+buff,(n+1)*size+buff,y*size+buff),fill=128)
                if y == 0:
                    if n != 0:
                        if value != values[y][n-1]:
                            draw.line((n*size+buff,y*size+buff,n*size+buff,(y+1)*size+buff),fill=128)
    im.show()       #Show the image
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
