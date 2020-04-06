import redis
import time
import io
import cv2
import numpy as np
from PIL import Image

face_cascade = cv2.CascadeClassifier('../../FaceDetection/haarcascade_frontalface_default.xml')
eye_cascade  = cv2.CascadeClassifier('../../FaceDetection/haarcascade_eye.xml')

r = redis.Redis(host='localhost', port=6379, db=0)

count = 0
while True:

    simg = r.xread({'camera:0':'$'},count=1,block=0)
    try:
        img_id = simg[0][1][0][0]
        print(img_id)
        data = io.BytesIO(simg[0][1][0][1][b'image'])
    except:
        pass
    img   = Image.open(data)
    arr   = np.array(img)
    arr   = cv2.cvtColor(arr, cv2.COLOR_BGR2RGB)
    gray  = cv2.cvtColor(arr, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(gray)

    facesboxes = []
    eyesboxes  = []
    facecounter= 0
    eyecounter = 0

    for (x,y,w,h) in faces:
       facecounter +=1

       cv2.rectangle(arr,(x,y),(x+w,y+h),(255,0,0),2)
       facesboxes += [int(x), int(y), int(x+w), int(y+h)]

       #roi_gray  = gray[y:y+h, x:x+w]
       #roi_color = arr[y:y+h, x:x+w]

       #eyes=eye_cascade.detectMultiScale(roi_gray)
       eyes=eye_cascade.detectMultiScale(gray)

       for(ex,ey,ew,eh) in eyes:
           eyecounter += 1
           if eyecounter > 2: # Just sent two eyse
               eyecounter = 2
               break
           eyesboxes   += [int(ex), int(ey), int(ex+ew), int(ey+eh)]
           #cv2.rectangle(roi_color,(ex, ey),(ex+ew, ey+eh),(0,255,0),2)

    if facesboxes:
        msg = {
            'facesboxes' : str(facesboxes),
            'facecounter': int(facecounter),
            'eyesboxes'  : str(eyesboxes),
            'eyecounter' : int(eyecounter),
            'ref_id'     : img_id
        }
        _id = r.xadd('camera:0:facedect',msg)

    #name = 'outputtest'+str(count)+'.jpg'
    #cv2.imwrite(name,arr)
    #count += 1

