import MTLabFace
from PIL import Image, ImageDraw


image=Image.open('./test.jpg')
im_draw = ImageDraw.Draw(image)


config=MTLabFace.MTFaceConfig()
config.MeituFA=True
result=MTLabFace.FaceFeatureDetect(image,config) #result are faces in the image


for i in range(len(result)):
    face=result[i]
    Rect=face["Rect"]
    Points=face["Points"]   # fa will return 118 points

    for i in range(len(Points)):
        pos = Points[i]
        im_draw.point(pos)

    image.show()
