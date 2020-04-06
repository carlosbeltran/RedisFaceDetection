# RedisFaceDetection

How to use redis to run remotely a haar face detector
This demo implements a producer/consumer pattern to distribute an image processing algorithm in a remote computer.
A script reads an image that is shared with face detection script by means of a Radis database.
The following picture describes a two computers architecture; naturaly, all the modules could also run in the same
computer or in three different computers.

<img src="./architecture.png" width="650" height="500">

## How to use (local)

The Redis database can be run opening a terminal and using the provide docker-compose file:

```bash
$ docker-compose up
```
In other terminals you can run the detection script:

```bash
$ python3 facedecteng.py
```
and the capture script:

```bash
$ python3 capture.py
```

## How to use (distributed)

The same as in local but taking care of setting the correct paths for the Redis database.
