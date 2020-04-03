import redis
import time

r = redis.Redis(host='localhost', port=6379, db=0)
ps = r.pubsub()
ps.subscribe('test')
#new_msg = ps.get_message()['data']
#print (new_msg)
while True:
    try:
        message = ps.get_message()
    except redis.ConnectionError:
        # Do reconnection attempts here such as sleeping and retrying
        ps = r.pubsub()
        ps.subscribe('test')
    if message:
        # do something with the message
        print(message)
    time.sleep(0.1)
