from multiprocessing import shared_memory
import ctypes
FRAME_SIZE = 4092
MUTEX_SIZE = 40  
TOTAL_SIZE = MUTEX_SIZE + FRAME_SIZE

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