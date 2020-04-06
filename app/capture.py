import redis
import cv2

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

print(boxes[0][1][0][1][b'facesboxes'])
print(boxes[0][1][0][1][b'eyesboxes'])
