import redis
import time
import io
import cv2
import numpy as np
from PIL import Image


r = redis.Redis(host='localhost', port=6379, db=0)
ps = r.pubsub()
ps.subscribe('images')
#new_msg = ps.get_message()['data']
#print (new_msg)
count = 0
while True:
    for message in ps.listen():
        print(message)
        img_id = message['data']
        simg = r.xread({'camera:0':img_id})
        #print(simg)
        try:
            data = io.BytesIO(simg[0][1][0][1][b'image'])
        except:
            pass
        img = Image.open(data)
        arr = np.array(img)
        arr = cv2.cvtColor(arr, cv2.COLOR_BGR2RGB)
        name = 'outputtest'+str(count)+'.jpg'
        cv2.imwrite(name,arr)
        count += 1

