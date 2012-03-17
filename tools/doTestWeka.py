import shutil
import os
import subprocess
import time

def writeHeader(outfile):
    outfile.write("@Relation emotion\n")
    outfile.write("@Attribute pitch_avg numeric\n")
    outfile.write("@Attribute pitch_min numeric\n")
    outfile.write("@Attribute pitch_max numeric\n")
    outfile.write("@Attribute pitch_range numeric\n")
    outfile.write("@Attribute speech_energy numeric\n")
    outfile.write("@Attribute class {happiness, sadness}\n\n")

    outfile.write("@Data\n")  

def runEmotionTrial(subject, emotion, outfile):
    average_p = []
    min_p = []
    max_p = []

    #outfile.write("\n-----Emotion: " + emotion + '-----\n')
    
    fullpath = "/home/gareth/BrainLab/berlin-speech-database/segregated/" + subject + '/' + emotion 
    samples = os.listdir(fullpath)
    
    for sample in samples:
        sample_path = fullpath + '/' + sample

        shutil.copy(sample_path, '/home/gareth/BrainLab/emotional-speech/data/in.wav') 
        print "running trial: " + sample_path 

        # run it
        runTrial(outfile, emotion)


def main():
    outfile = open("emote.arff", 'w')
    writeHeader(outfile)
    
    test_subjects = os.listdir("/home/gareth/BrainLab/berlin-speech-database/segregated")

    for subject in test_subjects:
        if subject.find('Female') != -1:
            runEmotionTrial(subject, "happiness", outfile)
            runEmotionTrial(subject, "sadness", outfile)

    outfile.close()

def runTrial(outfile, emotion):
    process_path = "../build/bin/emote"
    process = subprocess.Popen([process_path], shell=False, stdin = subprocess.PIPE, stdout = subprocess.PIPE)
    result = process.communicate()[0]
    result = result.split()
    
    pitches = []
    energies = []

    for line in result:
        if line.find(',') != -1:
            line = line.split(',')
            if float(line[0]) > 0:
                pitches.append(float(line[0]))
                energies.append(float(line[2]))
                

    # Save pitch stats
    if len(pitches) != 0:
        avg_p = float(sum(pitches)) / len(pitches)
        max_p = max(pitches)
        min_p = min(pitches)
        range_p = max_p - min_p

        avg_energy = float(sum(energies)) / len(energies)

        print "\naverage_pitch: " + str(avg_p)                  
        print "max_pitch: " + str(max(pitches))
        print "min_pitch: " + str(min(pitches))
        print "pitch range: " + str(range_p)
        print "average_energy: " + str(avg_energy)

        outfile.write(str(avg_p) + ", " + str(min_p) + ", " + str(max_p) + ", " + str(range_p) + ", " + str(avg_energy) + ", " + emotion + "\n")

    else:
        #outfile.write("Error with last sample\n")
        print "Error with last sample"

if __name__ == "__main__":
    main()
