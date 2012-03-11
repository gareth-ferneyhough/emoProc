import os
import sys

def readFeatures(filename):
    f = open(filename) 
    a = []

    for line in f:
        a.append(line.rstrip('\n'))

    f.close()
    return a

def writeFeatures(b):
    f = open("labeled-results.txt", 'w')
    for line in b:
        f.write(line)
        f.write("\n")

    f.close()

def getLabels(dir):
    labels = []
    filenames = []
    for root, dir, files in os.walk(dir):
        for file in files:
            filenames.append(file)

    filenames = sorted(filenames)
    for file in filenames:
        labels.append(file[5])

    ids = []
    for label in labels:
        if label == 'W':
            ids.append(1)
        if label == 'A':
            ids.append(2)
        if label == 'L':
            ids.append(3)
        if label == 'E':
            ids.append(4)
        if label == 'F':
            ids.append(5)
        if label == 'T':
            ids.append(6)
        if label == 'N':
            ids.append(7)

    return ids

def labelFeatures(labels, features):
    index = 0
    labeled_features = []

    for line in features:
        if line != '':
            if int(labels[index]) != int(line[7]):
                labeled_features.append('Wrong! ' + str(labels[index]) + ' ' + line)
            
            else:
                labeled_features.append(str(labels[index]) + ' ' + line)

            index += 1
        
#labeled_features.append(str(labels[index]) + ' ' + line)

    if index != len(labels):
        print "Tooooo many utterances detected. Not enough labels to match."

    return labeled_features

## Main starts here
if len(sys.argv) != 2:
    print "Usage: blah (class-results)"
    sys.exit()

features = readFeatures(sys.argv[1])

labels = getLabels("/home/gareth/BrainLab/berlin-speech-database/wav-2-sec/happy-sad-male/test")

#print labels
labeled = labelFeatures(labels, features)                     
#print labeled
writeFeatures(labeled)
