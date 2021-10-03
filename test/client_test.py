import subprocess
import threading

def main():
    print("Running client-server test...")
    server = subprocess.Popen('./run.sh', stdout=subprocess.PIPE)
    data = server.communicate()
    print(data)
    

if __name__ == "__main__":
    main()
