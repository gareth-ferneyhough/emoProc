import subprocess
import time


def main():
    process_path = "../build/bin/emote"
    process = subprocess.Popen([process_path], shell=False, stdin = subprocess.PIPE, stdout = subprocess.PIPE)
    #result = process.communicate('q')[0]
    #result = result.split()
    #time.sleep(5)
    #process.kill()
    
    while (True):
        output = process.stdout.readline()
        print output,

if __name__ == "__main__":
    main()
