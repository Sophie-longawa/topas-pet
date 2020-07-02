
from topas2numpy import read_ntuple
import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from skimage.draw import line
from matplotlib.pyplot import figure


figure(num=None, figsize=(16, 16), dpi=80, facecolor='w', edgecolor='k')

matplotlib.rcParams['agg.path.chunksize'] = 1000000000

phase_space_results = read_ntuple('PETPhaseSpace.phsp')
phase_space_results = np.array([list(x) for x in phase_space_results])#[0:100]
print(phase_space_results)

# Data for plotting
x_positions = phase_space_results[:,0]
y_positions = phase_space_results[:,1]
z_positions = phase_space_results[:,2]

"""
fig, ax = plt.subplots()
ax.scatter(x_positions, y_positions)

ax.set(xlabel='x (cm)', ylabel='y (cm)',
       title='Positions of incidences')
ax.grid()

fig.savefig("test.png")
plt.show()
"""


#make heat map

hlx = 250 #cm
hly = 250 #cm

detector_radius = 1.9 #m

resolution = .5 #cm

stddev = 1 #standard deviation for TOF detection

c = 299792458 #m/s

bins = np.zeros(shape=(int(2*hlx/resolution)+1,int(2*hly/resolution)+1))

time_window = 8 * 10**(-9) # 6 ns

times = np.add(phase_space_results[:,10], (phase_space_results[:,11] * (10**(-9)))) #topas time of start + time of flight converted to seconds from ns
energies = phase_space_results[:,5] #MeV

time_bins = [[] for i in range(int(times[-1]/ (time_window/2)) + 2)]

for i in range(len(times)):
	time_index = int(times[i] / (time_window/2))
	if energies[i] >= .5:
		time_bins[time_index].append(i)
		time_bins[time_index+1].append(i)

used_indexes = set([])

for i in range(0,len(time_bins)-1):
	hits_in_time_bin = list(dict.fromkeys(time_bins[i]+time_bins[i+1])) #remove dupes from merged lists
	#print(hits_in_time_bin)
	if len(hits_in_time_bin) == 2 and all([x not in used_indexes for x in hits_in_time_bin]):
		#print("found a coincidence!")

		i1, i2 = hits_in_time_bin

		used_indexes.add(i1)
		used_indexes.add(i2)

		#draw line with Bresenham's line algorithm
		#m = (y_positions[i1]-y_positions[i2])/(x_positions[i1]-x_positions[i2])

		#map coordinates into boxes
		x0 = int((x_positions[i1]+hlx)/resolution)
		y0 = int((y_positions[i1]+hly)/resolution)
		xf = int((x_positions[i2]+hlx)/resolution)
		yf = int((y_positions[i2]+hly)/resolution)

		rr, cc = line(x0, y0, xf, yf)

		#print(rr,cc)

		#TOF calculation
		distance_between_sensors = ((x_positions[i1]-x_positions[i2])**2 + (y_positions[i1]-y_positions[i2])**2 + (z_positions[i1]-z_positions[i2])**2)**.5

		if distance_between_sensors == 0:
			print("skipping distance of zero between sensors")
			continue

		delta_dist = c*(times[i2]-times[i1]) * (10 ** (2)) #d=ct in cm

		#max_time_diff = distance_between_sensors / c * (10 ** (-9)) #ns

		m = len(rr)/2 - len(rr) *  delta_dist/(distance_between_sensors) /2 #(times[i2]-times[i1])/max_time_diff * len(rr)/2 

		print(times[i2], times[i1])
		print(delta_dist/(distance_between_sensors))

		#make values for what to add based on normal distribution
		vals = np.array( [ 1/(stddev * (2*np.pi)**.5) * np.exp(-.5 * ((i-m)/stddev) ** 2 ) for i in range(0,len(rr)) ] )

		#print(vals)

		bins[rr, cc] += vals

		#print(rr)


print(bins)

plt.imshow(bins, cmap='hot', interpolation='nearest')
plt.savefig('PET_image.png')
plt.show()

