import numpy as np
import matplotlib.image as mpimg
import matplotlib.pyplot as plt



gain = 255 /(1920*1080-1)


cdf = np.load('./outcdf.npy')


h = np.zeros((256, 1))

for i in range(256):
    h[i] = (cdf[i]-1) * gain

my_eq_G = np.zeros((1080, 1920))

image = mpimg.imread('./test_1080p.bmp')



for i in range(1080):
 	for j in range(1920):
 		pix = image[i][j][1]
 		my_eq_G[i][j] = np.uint8(h[pix])


imgplot = plt.imshow(my_eq_G, cmap='gray')
plt.show()

print(cdf)