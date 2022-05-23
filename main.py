import cv2
import numpy as np
import serial

import  time
#端口，GNU / Linux上的/ dev / ttyUSB0 等 或 Windows上的 COM3 等
portx="COM8"
#波特率，标准值之一：50,75,110,134,150,200,300,600,1200,1800,2400,4800,9600,19200,38400,57600,115200
bps=115200
#超时设置,None：永远等待操作，0为立即返回请求结果，其他值为等待超时时间(单位为秒）
timex=5
# 打开串口，并得到串口对象
ser=serial.Serial(portx,bps,timeout=timex)
flag_x=-1 # max=188
flag_y=0 # max=120
flag_ok=0
count=0
text1=b'm'


img = np.zeros([120, 188], np.uint8)
cv2.namedWindow("enhanced",0);

cv2.resizeWindow("enhanced", 640, 480);

#
# # 读取图片。
# img = cv2.imread("2.jpg");
# # 转成灰度图片
# img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY);
# # 二值化
# ret,img=cv2.threshold(img, 127, 255, cv2.THRESH_BINARY);
#
#
#
#
temp3=''
while True:
    #print('ok')
    if ser.in_waiting:
        temp = ser.read()

        if temp==text1:
            temp4 = 0
            #print(temp)
            temp3 = ''
            temp2=ser.read(2820).hex()
            for i in temp2:
                temp5=bin(int(i, 16))[2:]
                temp3+=temp5.zfill(4)
            print(len(temp3))
            for i in range(len(img)):
                for j in range(len(img[i])):
                    #print(lists[i][j])
                    if temp3[temp4]=='1':
                        img[i][j] = 255
                    else:
                        img[i][j] = 0
                    temp4+=1
            print(img)
            cv2.imshow("enhanced", img)
            cv2.waitKey(300)
            #print(temp3)
        #print(temp)
    #     if temp==text1:
    #         print('ok')
    #         flag_x = -1
    #         flag_y = 0
    #     else:
    #
    #         temp = temp.hex()
    #         #print(temp)
    #         temp1 = '{:08b}'.format(int(temp, 16))
    #         temp1 = temp1[::-1]
    #         if count==23:
    #             for i in range(4):
    #                 if temp1[i] == '0':
    #                     flag_x += 1
    #                     img[flag_y][flag_x] = 0
    #
    #                 elif temp1[i] == '1':
    #                     flag_x += 1
    #                     img[flag_y][flag_x] = 255
    #         else:
    #             for i in range(8):
    #                 if temp1[i] == '0':
    #                     flag_x += 1
    #                     img[flag_y][flag_x] = 0
    #
    #                 elif temp1[i] == '1':
    #                     flag_x += 1
    #                     img[flag_y][flag_x] = 255
    #
    #         count += 1
    #         if count == 24:
    #             count = 0
    #             flag_x = -1
    #             flag_y += 1
    #
    # else:
    #     cv2.imshow("enhanced", img)
    #     cv2.waitKey(300)


      #print('{:08b}'.format(int(temp, 16)))
    #   if temp=='0f':
    #       flag_ok=1
    #       print('输出图片')
    #   if temp=='02':
    #       flag_ok=2
    #       flag_x=-1
    #       flag_y=0
    #       print('reset')
    #   if flag_ok == 1:
    #
    #       if temp == 'ff':
    #           flag_x += 1
    #           img[flag_y][flag_x]=255
    #           print(1)
    #       elif temp == '00':
    #           flag_x += 1
    #           img[flag_y][flag_x] = 0
    #           print(2)
    #       if flag_x == 187:
    #           flag_y += 1
    #           flag_x = -1
    # else:
    #     cv2.imshow("enhanced", img)
    #     cv2.waitKey(300)


# 读取图片。
#img = cv2.imread("2.jpg");
# 转成灰度图片
#img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY);
# 二值化
#ret,img=cv2.threshold(img, 127, 255, cv2.THRESH_BINARY);
#img=[[0,0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0,0],[255,255,251,251,251,251,251,251,251,251],[255,255,251,251,251,251,251,251,251,251],[255,255,251,251,251,251,251,251,251,251],[255,255,251,251,251,251,251,251,251,251],
#             [255,255,251,251,251,251,251,251,251,251],[255,255,251,251,251,251,251,251,251,251],[255,255,251,251,251,251,251,251,251,251],[255,255,251,251,251,251,251,251,251,251]]

# img = np.zeros([20, 10], np.uint8)
# cv2.imshow("title", img);
# # # 进程不结束，一直保持显示状态
# cv2.waitKey(100);
#
# #销毁所有窗口
# cv2.destroyAllWindows();

# img=np.array([[0,0,0,0,0,0,0,0,0,0],
#      [0,0,0,0,0,0,0,0,0,0],
#      [255,255,251,251,251,251,251,251,251,251],
#      [255,255,251,251,251,251,251,251,251,251],
#      [255,255,251,251,251,251,251,251,251,251],
#      [255,255,251,251,251,251,251,251,251,251],
#      [255,255,251,251,251,251,251,251,251,251],
#      [255,255,251,251,251,251,251,251,251,251],
#      [255,255,251,251,251,251,251,251,251,251],
#      [255,255,251,251,251,251,251,251,251,251]])
#img[0]=[255,255,251,251,251,251,251,251,251,251]
# img[2]=[255,255,251,251,251,251,251,251,251,251]
# img[3]=[255,255,251,251,251,251,251,251,251,251]
# img[4]=[255,255,251,251,251,251,251,251,251,251]
# img[5]=[255,255,251,251,251,251,251,251,251,251]
# img[6]=[255,255,251,251,251,251,251,251,251,251]
# img[7]=[255,255,251,251,251,251,251,251,251,251]
# img[8]=[255,255,251,251,251,251,251,251,251,251]
# img[9]=[255,255,251,251,251,251,251,251,251,251]
# img[10]=[255,255,251,251,251,251,251,251,251,251]
# # 显示图像
# print(img)
#cv2.imshow("title", img);
# 进程不结束，一直保持显示状态
#cv2.waitKey(1000);

#销毁所有窗口
#cv2.destroyAllWindows();


