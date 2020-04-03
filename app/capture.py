import redis

r = redis.Redis(host='localhost', port=6379, db=0)
r.publish('test','testing the pub/sub communication')
