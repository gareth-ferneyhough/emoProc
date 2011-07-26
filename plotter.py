## Awesome speech signal plotter ##
## Gareth Ferneyhough ##

import matplotlib.pyplot as plt

## Read in data ##
f = open("build/raw.dat")
strs = f.read()
strs = strs.split()

raw_audio = []
for flt in strs:
    raw_audio.append(float(flt))

f = open("build/segment.dat")
strs = f.read()
strs = strs.split()

segment = []
for flt in strs:
    segment.append(float(flt))

f = open("build/pitch.dat")
strs = f.read()
strs = strs.split()

pitch = []
for flt in strs:
    pitch.append(float(flt))

## Resize segmentation to plot over signal ##
signal_len = len(raw_audio)
scale_factor = (signal_len) / (len(segment))
segment2 = []

for i in range(0, len(segment)):
    for j in range(i*scale_factor, i*scale_factor+scale_factor):
        segment2.append(segment[i])


## Remove trailing and pre-trailing? blank data
## First, find start of audio data and change axis range
found = False
start = 0
while (found == False and start != signal_len):
    if raw_audio[start] != 0.0:
        found = True
    else:
        start+=1

## Rewind just a bit
start -= 50

## Then, find end of data
found = False
end = signal_len -1
while (found == False and end != 0):
    if raw_audio[end] != 0.0:
        found = True
    else:
        end-=1

## Fast forward just a bit
end += 50

## Plot! ##
plt.subplot(211)
plt.plot(raw_audio)
plt.plot(segment2, 'r')
plt.xlim(start, end)
plt.ylim(-1.0, 1.5)

plt.subplot(212)
plt.plot(pitch)
plt.xlim(start/scale_factor, end/scale_factor)

plt.show()
