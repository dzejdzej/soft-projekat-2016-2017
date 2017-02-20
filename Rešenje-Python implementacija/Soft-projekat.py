
import numpy as np
import cv2


body_cascade= cv2.CascadeClassifier('haarcascade_fullbody.xml');
presli = 0
semafor = cv2.imread('semafor.png',0)
visina, sirina = semafor.shape[::-1]
thresh = 0.9
zaTacku = 0
cap = cv2.VideoCapture('video3.avi')
cv2.ocl.setUseOpenCL(False)
while True:
    ret,img = cap.read()
    if ret==False:
        break       
    cv2.rectangle(img, (-100,75), (400, -75), (128, 128, 128), -1)
    font = cv2.FONT_HERSHEY_COMPLEX
    cv2.putText(img, 'Counter: '+str(presli), (75, 50), font, 1,(255,255,255),4)   
    gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
    res = cv2.matchTemplate(gray,semafor,cv2.TM_CCOEFF_NORMED)   
    loc = np.where(res>=thresh)

    for pt in zip(*loc[::-1]):
        zaTacku = pt[1]
        cv2.rectangle(img,pt,(pt[0]+visina,pt[1]+sirina),(0,0,255),2)
       # cv2.imshow('Semafor',img)
        cv2.line(img,(pt[0]-250,pt[1]+150),(pt[0]+250,pt[1]+150),(0,255,0),5)
        #cv2.imshow('Linija',img);
    bodies = body_cascade.detectMultiScale(gray,1.3,5)
    for(x,y,w,h) in bodies:
        cv2.rectangle(img,(x,y),(x+w,y+h),(255,0,0),2)
        tackaX=int(round( x+(w/2),0))
       # print tackaX
        tackaY=int(round((y+h)-(0.30*h),0))
       # print tackaY
       #cv2.circle(img,(tackaX,tackaY), 5, (0,0,255), -1)
        if tackaY-(zaTacku+150)==0:
            presli+=1          
            cv2.putText(img, 'Counter: '+str(presli), (75, 50), font, 1,(255,255,255),4)  
    cv2.imshow('Prelazak pesaka na crveno svetlo',img)  
    
    k = cv2.waitKey(10) & 0xff
    if k == 27:
        break   
cap.release()
cv2.destroyAllWindows()
