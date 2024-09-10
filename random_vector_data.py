import numpy as np

np.random.seed(0)
data = np.random.rand(200, 2)  # 1000 points in 128-dimensional space
np.savetxt("data.txt", data)  # Save the data to a text file