import cv2
import numpy as np
from cv2 import aruco

def scale_to_width(img, width):
    h, w = img.shape[:2]
    height = round(h * (width / w))
    dst = cv2.resize(img, dsize=(width, height))

    return dst

aruco = cv2.aruco

img = cv2.imread('m0-photo.jpg')

dictionary = aruco.getPredefinedDictionary(aruco.DICT_4X4_250)
parameters =  aruco.DetectorParameters()
detector = aruco.ArucoDetector(dictionary, parameters)
corners, ids, rejectedImgPoints = detector.detectMarkers(img)
img_marked = aruco.drawDetectedMarkers(img.copy(), corners, ids)   # 検出結果をオーバーレイ

print(corners)

# resized_img = scale_to_width(img_marked, 600) #結果画像が見やすいようにリサイズ

# while True:
#     cv2.imshow("Result", resized_img) # 表示
#     key = cv2.waitKey(10) 