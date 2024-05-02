import socket
import sys
import threading

sock = socket.socket()
sock.connect(('127.0.0.1', 5050))

def recv():
    while True:
        a = sock.recv(5120)
        if len(a) == 0:
            sock.close()
            return 0
        try:
            print(a[3:].decode('utf-8'))
            print(a[:3])
        except Exception:
            pass

def send(name):
    while True:
        sock.send(f'\x00\x00\x00[{name}]:{input()}'.encode('utf-8'))


if len(sys.argv) == 1:
    t1 = threading.Thread( target = recv)
    t2 = threading.Thread( target = send, args=(input('Ваш ник: '),))
    t1.start()
    t2.start()
else:
    sock.send(b'\x01' + bytes(sys.argv[1], 'utf-8'))
    print(sock.recv(5120).decode('utf-8'))