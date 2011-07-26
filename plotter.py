import matplotlib.pyplot as plt

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

plt.subplot(311)
plt.plot(raw_audio)
plt.plot(segment, 'r')
ax = plt.gca()
ax.set_autoscale_on(False)
plt.xlim(0, len(raw_audio)-1)

plt.subplot(312)
plt.plot(segment)
ax = plt.gca()
ax.set_autoscale_on(False)
plt.xlim(0, len(segment)-1)

plt.subplot(313)
plt.plot(pitch)
ax = plt.gca()
ax.set_autoscale_on(False)
plt.xlim(0, len(pitch)-1)

plt.show()
