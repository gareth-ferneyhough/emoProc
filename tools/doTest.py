import shutil
import os
import subprocess
import time

def runEmotionTrial(subject, emotion, outfile):
    average_p = []
    min_p = []
    max_p = []

    outfile.write("\n-----Emotion: " + emotion + '-----\n')
    
    fullpath = "/home/gareth/BrainLab/berlin-speech-database/segregated/" + subject + '/' + emotion 
    samples = os.listdir(fullpath)
    
    for sample in samples:
        sample_path = fullpath + '/' + sample

        shutil.copy(sample_path, '/home/gareth/BrainLab/emotional-speech/data/in.wav') 
        print "running trial: " + sample_path 

        # run it
        runTrial(sample_path, outfile)

    #     # save the stuff
    #     average_p.append(pitch_mean)
    #     min_p.append(pitch_min)
    #     max_p.append(pitch_max)

    # outfile.write("\n--- Trial averages: ---\n")
    # total_mean = float(sum(average_p)) / len(average_p)
    # print "Trial mean


def main():
    outfile = open("out.txt", 'w')
    
    test_subjects = os.listdir("/home/gareth/BrainLab/berlin-speech-database/segregated")

    for subject in test_subjects:
        outfile.write("\n-------- Subject: " + subject + '--------\n')
        #runEmotionTrial(subject, "anger", outfile)
        #runEmotionTrial(subject, "anxiety_fear", outfile)
        runEmotionTrial(subject, "happiness", outfile)
        runEmotionTrial(subject, "sadness", outfile)

    outfile.close()

def runTrial(filename, outfile):
    process_path = "../build/bin/emote"
    process = subprocess.Popen([process_path], shell=False, stdin = subprocess.PIPE, stdout = subprocess.PIPE)
    result = process.communicate()[0]
    result = result.split()
    
    pitches = []
    for line in result:
        if line.find(',') != -1:
            #outfile.write(line + '\n')
            line = line.split(',')
            if float(line[0]) > 0:
                pitches.append(float(line[0]))

    # Save pitch stats
    if len(pitches) != 0:
        avg_p = float(sum(pitches)) / len(pitches)
        max_p = max(pitches)
        min_p = min(pitches)

        print "\naverage_pitch: " + str(avg_p)                  
        print "max_pitch: " + str(max(pitches))
        print "min_pitch: " + str(min(pitches))

        outfile.write("\naverage_pitch: " + str(avg_p) + '\n')                  
        outfile.write("max_pitch: " + str(max(pitches)) + '\n')
        outfile.write("min_pitch: " + str(min(pitches)) + '\n')

    else:
        outfile.write("Error with last sample\n")
        print "Error with last sample"

if __name__ == "__main__":
    main()
