import os

def readFeatures():
    f = open("build/features.txt")
    a = []

    for line in f:
        a.append(line.rstrip('\n'))

    f.close()
    return a

def writeFeatures(b):
    f = open("build/labeled-features.txt", 'w')
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
        if label == 'L':
            ids.append(2)
        if label == 'E':
            ids.append(3)
        if label == 'A':
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
        if line == "####":
            index += 1
        else:
            labeled_features.append(str(labels[index]) + ' ' + line)    

    return labeled_features

## Main starts here
features = readFeatures()
labels = getLabels("/home/gareth/BrainLab/berlin-speech-database/wav/trial1")
print labels
labeled = labelFeatures(labels, features)                     
writeFeatures(labeled)
