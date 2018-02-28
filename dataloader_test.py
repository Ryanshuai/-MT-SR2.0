from PIL import Image


from Face2Python.demo.mouth import get_mouth_mask

im = Image.open('test.jpg')

mask = get_mouth_mask(im)
mask.show()
