import cv2
import numpy as np
from cv2 import aruco

#固定値
ROBOT_MARKER_ID = 0
TARGET_MARKER_ID = 1
IMAGE_URL = 'm1-photo.jpg'

def getMarkerPositions(frame, targets):
    # 変数の定義
    positions = []
    dictionary = aruco.getPredefinedDictionary(aruco.DICT_4X4_250)
    parameters =  aruco.DetectorParameters()
    detector = aruco.ArucoDetector(dictionary, parameters)

    # マーカー情報の取得
    corners, ids, _ = detector.detectMarkers(frame)

    # 座標の検出
    if ids is not None and len(ids) > 0:
        for id in targets:
            # マーカーの頂点座標を取得
            target_id_index = np.where(ids == id)[0]
            if len(target_id_index) > 0:
                target_corners = corners[target_id_index[0]][0]
                # マーカーの中心座標を取得
                target_width, target_height = target_corners.mean(axis=0)
                # 結果をpositionsに追加
                positions.append([id, [target_width, target_height]])

    return positions

img = cv2.imread(IMAGE_URL)
marker_pos = getMarkerPositions(img, [ROBOT_MARKER_ID, TARGET_MARKER_ID])


# # 画像をリサイズする関数
# def scale_to_width(img, width):
#     h, w = img.shape[:2]
#     height = round(h * (width / w))
#     dst = cv2.resize(img, dsize=(width, height))

#     return dst

# # 変数の定義
# aruco = cv2.aruco
# img = cv2.imread(IMAGE_URL)

# # マーカー情報の取得
# dictionary = aruco.getPredefinedDictionary(aruco.DICT_4X4_250)
# parameters =  aruco.DetectorParameters()
# detector = aruco.ArucoDetector(dictionary, parameters)
# corners, ids, rejectedImgPoints = detector.detectMarkers(img)

# # ロボットのコーナー座標を格納
# robot_id_index = np.where(ids == ROBOT_MARKER_ID)[0][0]
# robot_corners = corners[robot_id_index][0]

# # ロボットの中心座標を取得
# robot_width, robot_height = robot_corners.mean(axis=0)

# # 検出結果をオーバーレイ
# img_marked = aruco.drawDetectedMarkers(img.copy(), corners, ids)
# cv2.circle(img_marked, (int(robot_width), int(robot_height)), 15, (0,0,255), -1)

# # 結果画像が見やすいようにリサイズ
# resized_img = scale_to_width(img_marked, 600)

# # 結果の表示
# while True:
#     cv2.imshow("Result", resized_img)
#     key = cv2.waitKey(10) 