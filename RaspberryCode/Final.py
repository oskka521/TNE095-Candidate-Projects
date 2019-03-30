from picamera.array import PiRGBArray
from picamera import PiCamera
import time
import sys
import cv2
import numpy as np
import smbus
import cv2.aruco as aruco
import RPi.GPIO as GPIO
import os
import serial




##
##with open('calibration.yaml') as f:
##    loadeddict = yaml.load(f)
##    camera_matrix = np.asarray(loadeddict.get('camera_matrix'))
##    dist_coeffs= np.asarray(loadeddict.get('dist_coeff'))


######################################CAMERA#############################
def nothing(x):
    pass

def isset(v):
    try: 
        type (eval(v)) 
    except: 
        return 0 
    else: 
        return 1



#cv2.namedWindow("Trackbar")
#cv2.createTrackbar("L-H","Trackbar",0,179,nothing)
#cv2.createTrackbar("L-S","Trackbar",0,255,nothing)
#cv2.createTrackbar("L-V","Trackbar",0,255,nothing)
#cv2.createTrackbar("U-H","Trackbar",0,179,nothing)
#cv2.createTrackbar("U-S","Trackbar",0,255,nothing)
#cv2.createTrackbar("U-V","Trackbar",0,255,nothing)
#cv2.createTrackbar("Thresh","Trackbar",0,5000,nothing)

######################################I2C#############################
def RunI2C(QR):
    print ("Starting I2C")
    bus = smbus.SMBus(1)
    address = 0x07
    def writeNumber(value):
        bus.write_byte(address, value)
        return -1

    def readNumber():
        number = bus.read_byte_data(adress,1)
        print("RECV")
        return number

    data = QR
    data_list = list(data)
    for i in data_list:
        writeNumber(int(ord(i)))
        time.sleep(.1)

    writeNumber(int(0x0A))

def Uart(QR):
    port = serial.Serial("/dev/ttyS0",115200)
    port.write(QR.encode())
    port.close()
    
      
camera = PiCamera()
camera.resolution = (368, 368)
rawCapture = PiRGBArray(camera, size=(368, 368))
camera.rotation = 180
time.sleep(0.1)
print ("PiCamera ready RUNNING COLOR AND QR")
print ("OpenCV version: %s" % (cv2.__version__))
print ("Press q to exit ...")
bilOld = ""

for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):

    #l_h = cv2.getTrackbarPos("L-H", "Trackbar")
    #l_s = cv2.getTrackbarPos("L-S", "Trackbar")
    #l_v = cv2.getTrackbarPos("L-V", "Trackbar")
    #u_h = cv2.getTrackbarPos("U-H", "Trackbar")
    #u_s = cv2.getTrackbarPos("U-S", "Trackbar")
    #u_v = cv2.getTrackbarPos("U-V", "Trackbar")
    #TH = cv2.getTrackbarPos("Thresh","Trackbar")

    output = frame.array.copy()
    #imageB = frame.array
    #frame = cv2.blur(imageB, (3,3))
    #hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    #lower_Blue = np.array([l_h,l_s,l_v])
    #upper_Blue = np.array([u_h,u_s,u_v])
        
    #thresh = cv2.inRange(hsv,lower_Blue,upper_Blue)
    #thresh2 = thresh.copy()
    #image, contours,hierarchy = cv2.findContours(thresh,cv2.RETR_LIST,cv2.CHAIN_APPROX_SIMPLE)

    #x = 0
    #max_area = TH
    #for cnt in contours:
    #    area = cv2.contourArea(cnt)
    #    if area > max_area:
    #        max_area = area
     #       best_cnt = cnt
    #        x = 1

    #if isset('best_cnt') & x==1:
    #    M = cv2.moments(best_cnt)
    #    cx,cy = int(M['m10']/M['m00']), int(M['m01']/M['m00'])
    #    cv2.circle(frame,(cx,cy),10,255,-1)
    #    print ("Central pos: (%d, %d)" % (cx,cy))
        #else:
                #print "[Warning]Tag lost..."

    gray = cv2.cvtColor(output, cv2.COLOR_BGR2GRAY, dstCn=0)
    aruco_dict = aruco.Dictionary_get(aruco.DICT_6X6_250)
    parameters = aruco.DetectorParameters_create()
    corners, ids, rejetedImgPointers = aruco.detectMarkers(gray,aruco_dict,parameters = parameters)

    
    
    
    
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(24, GPIO.IN)
            # 3 siffror fordon
        # 101 fram
        # 102 höger
        # 103 bak
        # 104 vänster
        # 2 siffror vägskylt
    stoppar = 0
    bil = str(ids).strip('[]')
    
    if(bil != "None"):
        length = len(ids)
        #print("len(ids): ", len(ids))
        #print ("corners: ", corners)
        
        DISarray = []
        for i in range(0,length):
            dx = str(corners[i][0][0][0]).strip('[]')
            dy = str(corners[i][0][2][0]).strip('[]')
            ddx = float(dx)
            ddy = float(dy)
            dist = abs(ddx-ddy)
            DISarray.insert(i, dist)
        print ("dists: ", dist)
        #print ("ids: ", ids)
        #print("DISarray: ", DISarray)
        idIndex = DISarray.index( max(DISarray) )
        #print("idIndex = ", idIndex)
        #print("ids[idIndex] = ", ids[idIndex])
    

        bil = str(ids[idIndex]).strip('[]')
        if(bil == "101" or bil == "102" or bil == "103" or bil == "104"):
            if(bil != bilOld):
                if(DISarray[idIndex] > 20):
                    print("Sending stopp")
                    print(bil)
                    Uart("STOP")
                    bilOld = bil
                    
        if(bil != bilOld):
            if(DISarray[idIndex] > 58):
                print("Sending")
                print(bil)
                Uart(bil)
                bilOld = bil

    
        
        




    

    GPIO.cleanup()
 
        
    gray = aruco.drawDetectedMarkers(output,corners,ids,(0,0,255))
    cv2.imshow('frame', output)
    #cv2.imshow('thresh', thresh2)

    key = cv2.waitKey(1) & 0xFF
    rawCapture.truncate(0)
 
    if key == ord("q"):
        break  

cv2.destroyAllWindows()
cap.release()
            
