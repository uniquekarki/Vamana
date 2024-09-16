import numpy as np

np.random.seed(0)
data = np.random.rand(100, 128)  # 1000 points in 128-dimensional space
np.savetxt("data/data.txt", data)  # Save the data to a text file