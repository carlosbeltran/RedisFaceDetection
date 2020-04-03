import redis
import time

r = redis.Redis(host='localhost', port=6379, db=0)
ps = r.pubsub()
ps.subscribe('test')
#new_msg = ps.get_message()['data']
#print (new_msg)
while True:
    for message in ps.listen():
        print(message)
