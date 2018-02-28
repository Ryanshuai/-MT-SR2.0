from FDFA import MTLabFace
from PIL import Image, ImageDraw
from torchvision.transforms import ToPILImage, ToTensor


def get_mouth_mask(im):
    if not Image.isImageType(im):
        im = ToPILImage()(im)

    mask = Image.new('RGB', im.size, (0,0,0))
    mask_draw = ImageDraw.Draw(mask)


    config=MTLabFace.MTFaceConfig()
    config.MeituFA=True
    result=MTLabFace.FaceFeatureDetect(im, config) #result are faces in the image

    face = result[0] #only one face, no need for
    Rect=face["Rect"]
    Points=face["Points"]   # fa will return 118 points


    mouth_points = Points[86:98]
    mp = mouth_points

    left, up, right, low = im.width, im.height, 0, 0
    for i in range(len(mouth_points)):
        pos = mouth_points[i]
        left = min(pos[0], left)
        up = min(pos[1], up)
        right = max(pos[0], right)
        low = max(pos[1], low)



    # fill with white up and down the mouth
    for i in range(6):
        x0, y0, x1, y1 = mp[i][0], mp[i][1], mp[i+1][0], mp[i+1][1]
        mask_draw.polygon([x0, y0, x1, y1, x1, 0, x0, 0], fill="white")
    for i in range(6, 11):
        x0, y0, x1, y1 = mp[i][0], mp[i][1], mp[i+1][0], mp[i+1][1]
        mask_draw.polygon([x0, y0, x1, y1, x1, im.height, x0, im.height], fill="white")
    x0, y0, x1, y1 = mp[11][0], mp[11][1], mp[0][0], mp[0][1]
    mask_draw.polygon([x0, y0, x1, y1, x1, im.height, x0, im.height], fill="white")

    # fill with white left and right
    mask_draw.rectangle([0, 0, left, im.height], fill="white")
    mask_draw.rectangle([right, 0, im.width, im.height], fill="white")

    mask = ToTensor()(mask)
    return mask

if __name__ == '__main__':
    im = Image.open('test.jpg')
    mask = get_mouth_mask(im)
    mask = ToPILImage()(mask)
    mask.show()