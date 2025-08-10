import socket
import threading

HOST = "127.0.0.1"
PORT = 5000
BUFFER_SIZE = 4096

clients = set()

def handle_incoming(sock:socket.socket):
    while True:
        try:
            data, addr = sock.recvfrom(BUFFER_SIZE)
            if addr not in clients:
                clients.add(addr)
                print("client: ",addr,"connected!")
            # Broadcast to all clients
            for client in clients:
                if client != addr:
                    sock.sendto(data, client)
        except Exception as e:
            print(f"[ERROR] {e}")

def main():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((HOST, PORT))
    print(f"[SERVER] UDP server started on {HOST}:{PORT}")
    threading.Thread(target=handle_incoming, args=(sock,), daemon=True).start()
    try:
        while True:
            pass
    except KeyboardInterrupt:
        print("\n[SERVER] Shutting down...")
if __name__ == "__main__":
    main()
