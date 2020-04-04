import redis
import time
import io
import cv2
import numpy as np
from PIL import Image

face_cascade = cv2.CascadeClassifier('../../FaceDetection/haarcascade_frontalface_default.xml')
eye_cascade = cv2.CascadeClassifier('../../FaceDetection/haarcascade_eye.xml')

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
        img  = Image.open(data)
        arr  = np.array(img)
        arr  = cv2.cvtColor(arr, cv2.COLOR_BGR2RGB)
        gray = cv2.cvtColor(arr, cv2.COLOR_BGR2GRAY)
        faces = face_cascade.detectMultiScale(gray)

        boxes = []
        counter = 0

        for (x,y,w,h) in faces:
           counter +=1
           cv2.rectangle(arr,(x,y),(x+w,y+h),(255,0,0),2)
           # font=cv2.FONT_HERSHEY_SIMPLEX
           #cv2.putText(img,'Jeevy',(x+w/2,y+h/2),font,0.5,(0,0,255),1,cv2.LINE_AA)
           #print(x,y)
           roi_gray  = gray[y:y+h, x:x+w]
           roi_color = arr[y:y+h, x:x+w]
           eyes=eye_cascade.detectMultiScale(roi_gray)
           # smiles=smile_cascade.detectMultiScale(roi_gray)
           for(ex,ey,ew,eh) in eyes:
               cv2.rectangle(roi_color,(ex, ey),(ex+ew, ey+eh),(0,255,0),2)

        name = 'outputtest'+str(count)+'.jpg'
        cv2.imwrite(name,arr)
        count += 1

