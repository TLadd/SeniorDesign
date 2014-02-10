import cv2
from cv2 import *
import os
import glob

#TODO: Implement loop to convert jpgs to avi

img1 = cv2.imread('1.jpg')
height , width , layers =  img1.shape
video = cv2.VideoWriter('video.avi', cv.CV_FOURCC('M','J','P','G'),30,(width,height))

#video.write(img1)
#video.write(img2)
#video.write(img3)

filelist = glob.glob(os.path.join('./v/', '*'))
prevTime = 0
intCnt = 0

for filename in sorted(filelist, cmp=lambda x,y: 
				2*cmp(float(x[6:-4].split(".")[0]), float(y[6:-4].split(".")[0])) +
				cmp(float(x[6:-4].split(".")[1]), float(y[6:-4].split(".")[1]))):
	#print filename
	time = filename[6:]
	time = time[0:-4]
	#if(len(time) != 8):
	#	continue
	time = float(time)
	print(intCnt, ": ", filename)
	#print time - prevTime
	
	prevTime = time
	cv2.imwrite('./v_cnt/'+str(intCnt)+".jpg", cv2.imread(filename))
	intCnt += 1
	video.write(cv2.imread(filename))

cv2.destroyAllWindows()
video.release()
