import redis
import cv2
import numpy as np

r = redis.Redis(host='localhost', port=6379, db=0)
#ps = r.pubsub()
#ps.subscribe('boxes')
img=cv2.imread('../girl_small.jpg')

count = 1
_, data = cv2.imencode('.jpg', img)
msg = {
    'count': count,
    'image': data.tobytes()
}
count += 1
_id = r.xadd('camera:0', msg, maxlen=10000)
boxes = r.xread({'camera:0:facedect':'$'},count=1,block=0)

faceboxes = boxes[0][1][0][1][b'facesboxes']
eyesboxes = boxes[0][1][0][1][b'eyesboxes']
n_fboxes  = int(boxes[0][1][0][1][b'facecounter'])
n_eboxes  = int(boxes[0][1][0][1][b'eyecounter'])
img_id    = boxes[0][1][0][1][b'ref_id']

fboxes    = np.fromstring(faceboxes.decode('utf-8')[1:-1], sep=',')
eboxes    = np.fromstring(eyesboxes.decode('utf-8')[1:-1], sep=',')

print(fboxes)
print(eboxes)
print(faceboxes)
print(eyesboxes)

#draw = ImageDraw.Draw(img)

for nbox in range(n_fboxes):  # Draw boxes
    x1 = int(fboxes[nbox*4])
    y1 = int(fboxes[nbox*4+1])
    x2 = int(fboxes[nbox*4+2])
    y2 = int(fboxes[nbox*4+3])
    #draw.rectangle(((x1, y1), (x2, y2)), width=5, outline='blue')
    cv2.rectangle(img,(x1,y1),(x2,y2),(255,0,0),2)

for nbox in range(n_eboxes):  # Draw boxes
    x1 = int(eboxes[nbox*4])
    y1 = int(eboxes[nbox*4+1])
    x2 = int(eboxes[nbox*4+2])
    y2 = int(eboxes[nbox*4+3])
    #draw.rectangle(((x1, y1), (x2, y2)), width=5, outline='green')
    cv2.rectangle(img,(x1, y1),(x2, y2),(0,255,0),2)

name = 'girlhaarresult.jpg'
cv2.imwrite(name,img)
