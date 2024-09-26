import cv2
import numpy as np
from cv2 import aruco

#固定値
ROBOT_MARKER_ID = 0
TARGET_MARKER_ID = 1
IMAGE_URL = 'm1-photo.jpg'

def getMarkerPositions(img_url, targets):
    # 変数の定義
    aruco = cv2.aruco
    img = cv2.imread(img_url)
    positions = []
    # マーカー情報の取得
    dictionary = aruco.getPredefinedDictionary(aruco.DICT_4X4_250)
    parameters =  aruco.DetectorParameters()
    detector = aruco.ArucoDetector(dictionary, parameters)
    corners, ids = detector.detectMarkers(img)

    for id in targets:
        # マーカーの頂点座標を取得
        target_id_index = np.where(ids == id)[0][0]
        target_corners = corners[target_id_index][0]
        # マーカーの中心座標を取得
        target_width, target_height = target_corners.mean(axis=0)
        # 結果をpositionsに追加
        positions.append([id, [target_width, target_height]])

    return positions

marker_pos = getMarkerPositions(IMAGE_URL, [ROBOT_MARKER_ID, TARGET_MARKER_ID])