import socket,sys,threading
from shm_manager import manage_shm

SERVER_IP = "127.0.0.1"  
SERVER_PORT = 5000
BUFFER_SIZE = 4096
run=True

def send_loop(sock:socket.socket, shm_in:manage_shm):
    prev = ""
    global run
    while run:
        msg = shm_in.read()
        if msg and msg != prev:
            print(msg,"to python")
            sock.sendto(msg.encode('utf-8'), (SERVER_IP, SERVER_PORT))
            prev = msg

def recv_loop(sock:socket.socket, shm_out:manage_shm):
    global run
    while run:
        data, _ = sock.recvfrom(BUFFER_SIZE)
        text = data.decode('utf-8', errors='ignore')
        if(data=="terminate"):run=False
        print(f"[RECEIVED] {text}")
        shm_out.write(text)

def main():
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <send_shm_name> <recv_shm_name>")
        return
    
    send_name = sys.argv[1]  # name for shared memory you READ to
    recv_name = sys.argv[2]  # name for shared memory you WRITE to

    shm_in = manage_shm(send_name)  # gets from C++
    shm_out = manage_shm(recv_name)

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.setblocking(True)

    threading.Thread(target=send_loop, args=(sock, shm_in), daemon=True).start()
    threading.Thread(target=recv_loop, args=(sock, shm_out), daemon=True).start()

    try:
        while True:
            pass
    except KeyboardInterrupt:
        print("\n[CLIENT] Shutting down...")
        shm_in.close()
        shm_out.close()
        sock.close()

if __name__ == "__main__":
    main()
