from PIL import Image, ImageDraw


im = Image.new('RGBA', (400, 400), (255, 255, 255, 0)) 
draw = ImageDraw.Draw(im) 
draw.line((100,200, 150,300), fill=128)
im.show()