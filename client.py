import socket
import sys
import threading

sock = socket.socket()
sock.connect(('nekopara.ru', 5050))

a = input("Введите пароль: ")

def recv():
    while True:
        a = sock.recv(5120)
        if len(a) == 0:
            sock.close()
            return 0
        try:
            print(a[11:].decode('utf-8'))
            print(a[1:3])
        except Exception:
            pass

def send(name):
    global a
    while True:
        sock.send(b'\x00\x00\x00' + a + f'[{name}]:{input()}'.encode('utf-8'))


if len(sys.argv) == 1:
    t1 = threading.Thread( target = recv)
    t2 = threading.Thread( target = send, args=(input('Ваш ник: '),))
    t1.start()
    t2.start()
else:
    sock.send(b'\x01' + bytes(sys.argv[1], 'utf-8'))
    print(sock.recv(5120).decode('utf-8'))