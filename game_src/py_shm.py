import ctypes,threading
from multiprocessing import shared_memory
FRAME_SIZE = 4092
MUTEX_SIZE = 40  
TOTAL_SIZE = MUTEX_SIZE + FRAME_SIZE
run=True

class manage_shm():
    def __init__(self,name:str="shm"):
        self.shm=shared_memory.SharedMemory(name=name,create=False)
        class PThreadMutex(ctypes.Structure):
            _fields_ = [('_data', ctypes.c_byte*MUTEX_SIZE)]
        self.mutex = PThreadMutex.from_buffer(self.shm.buf)
        self.libc = ctypes.CDLL("libc.so.6")
        
    def read(self):
        self.libc.pthread_mutex_lock(ctypes.byref(self.mutex))
        raw_data = bytes(self.shm.buf[MUTEX_SIZE:TOTAL_SIZE])
        text = raw_data.split(b'\0', 1)[0].decode('utf-8', errors='ignore')
        self.libc.pthread_mutex_unlock(ctypes.byref(self.mutex))
        return text
    
    def write(self, data: str = "empty"):
        encoded = data.encode('utf-8')
        if len(encoded) >= FRAME_SIZE:
            raise ValueError("Data too long for shared memory")
        self.libc.pthread_mutex_lock(ctypes.byref(self.mutex))
        self.shm.buf[MUTEX_SIZE: MUTEX_SIZE + FRAME_SIZE] = b'\0' * FRAME_SIZE
        self.shm.buf[MUTEX_SIZE:MUTEX_SIZE + len(encoded)] = encoded
        self.libc.pthread_mutex_unlock(ctypes.byref(self.mutex))
    def close(self): 
        try:                                               ## Sometimes Python's Shared_memory default destructor having issues 
            del self.mutex                                 ## with shm.close() , hence the exception handling 
            self.shm.close()
        except BufferError as e:
            print(f"[WARNING] Shared memory close issue: {e}")
        except Exception as e:
            print(f"[ERROR] Unexpected exception in close: {e}")
def read_data(get_data: manage_shm):
    previous = "trsh"
    global run
    while run:
        test = get_data.read()
        if test != previous:
            print(test)
            if(test=="terminate"):run=False
            previous = test

def write_data(send_data:manage_shm):
    global run
    while run:
        try:
            line = input()
            send_data.write(line)
            if line == "terminate":
                run = False
        except Exception as e:
            print(f"[ERROR] in write_data: {e}")
            run = False
def shmpy():
    print("Python process started!")
    get_data = manage_shm("send_data")
    send_data = manage_shm("get_data")
    t_read = threading.Thread(target=read_data, args=(get_data,))
    t_write = threading.Thread(target=write_data, args=(send_data,))
    
    t_read.start()
    t_write.start()
    
    t_read.join()
    t_write.join()
    
    send_data.close() 
    get_data.close()
    
if __name__ == "__main__":
    shmpy()
