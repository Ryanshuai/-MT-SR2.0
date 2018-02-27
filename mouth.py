import MTLabFace
from PIL import Image, ImageDraw



def get_mouth_mask(im):

    mask = Image.new ('RGB', im.size, (255, 255, 255))
    mask_draw = ImageDraw.Draw(mask)

    config=MTLabFace.MTFaceConfig()
    config.MeituFA=True
    result=MTLabFace.FaceFeatureDetect(im, config) #result are faces in the image

    face = result[0] #only one face, no need for
    Rect=face["Rect"]
    Points=face["Points"]   # fa will return 118 points


    left, up, right, low = im.width, im.height, 0, 0
    mouth_points = Points[86:98]

    # get the relative_mouth_points
    for i in range(len(mouth_points)):
        pos = mouth_points[i]
        left = min(pos[0], left)
        up = min(pos[1], up)
        right = max(pos[0], right)
        low = max(pos[1], low)

    mp = mouth_points


    # fill with white up and down the mouth
    for i in range(6):
        x0, y0, x1, y1 = mp[i][0], mp[i][1], mp[i+1][0], mp[i+1][1]
        mask_draw.polygon([x0, y0, x1, y1, x1, 0, x0, 0], fill="black")
    for i in range(6, 11):
        x0, y0, x1, y1 = mp[i][0], mp[i][1], mp[i+1][0], mp[i+1][1]
        mask_draw.polygon([x0, y0, x1, y1, x1, im.height, x0, im.height], fill="black")
    x0, y0, x1, y1 = mp[11][0], mp[11][1], mp[0][0], mp[0][1]
    mask_draw.polygon([x0, y0, x1, y1, x1, im.height, x0, im.height], fill="black")

    # fill with white left and right
    mask_draw.rectangle([0, 0, left, im.height], fill="black")
    mask_draw.rectangle([right, 0, im.width, im.height], fill="black")

    return mask
