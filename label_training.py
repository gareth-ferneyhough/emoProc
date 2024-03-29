import os

def readFeatures():
    f = open("data/features.txt")
    a = []

    for line in f:
        a.append(line.rstrip('\n'))

    f.close()
    return a

def writeFeatures(b):
    f = open("data/labeled-features.txt", 'w')
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
            ids.append(1) # Anger
        if label == 'A':
            ids.append(2) # Fear
        if label == 'L':
            ids.append(3) # Boredom
        if label == 'E':
            ids.append(4) # Disgust
        if label == 'F':
            ids.append(5) # Happy
        if label == 'T':
            ids.append(6) # Sad
        if label == 'N':
            ids.append(7) # Neutral

    return ids

def labelFeatures(labels, features):
    index = 0
    labeled_features = []

    for line in features:
        if line == "####":
            index += 1
        else:
            labeled_features.append(str(labels[index]) + ' ' + line)

    if index != len(labels):
        print "Tooooo many utterances detected. Not enough labels to match."

    return labeled_features

## Main starts here
features = readFeatures()
#labels = getLabels("/home/gareth/BrainLab/berlin-speech-database/wav-2-sec/speaker1/happy-sad")
labels = getLabels("/home/gareth/BrainLab/berlin-speech-database/wav-2-sec/happy-sad-angry-fear-male/test1")
#print labels
labeled = labelFeatures(labels, features)                     
writeFeatures(labeled)
