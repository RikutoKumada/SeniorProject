import cv2
import numpy as np
from cv2 import aruco

# # 台形補正をする関数
# def warpPerspective(img, img_size):
#     # グレースケール変換
#     gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
#     # ガウシアンブラーでノイズ除去
#     blurred = cv2.GaussianBlur(gray, (7, 7), 0)
#     # エッジ検出 (Canny) - 閾値調整
#     edges = cv2.Canny(blurred, 50, 150)
#     # 輪郭の検出
#     contours, _ = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
#     # 最も大きな輪郭を探す (面積に基づくフィルタリング)
#     max_area = 0
#     screen_contour = None
#     # 机の頂点座標を取得
#     for contour in contours:
#         area = cv2.contourArea(contour)
#         if area > 1000:  # 面積が小さい輪郭は無視
#             approx = cv2.approxPolyDP(contour, 0.02 * cv2.arcLength(contour, True), True)
#             if len(approx) == 4 and area > max_area:  # 四角形かつ最大の面積
#                 screen_contour = approx
#                 max_area = area

#     if screen_contour is None:
#         print("机の輪郭が見つかりませんでした")
#         return None
#     else:
#         # 変形後画像サイズ
#         width, height = img_size
#         # 頂点の用意
#         marker_coordinates = np.float32(screen_contour)
#         true_coordinates   = np.float32([[0,height], [width,height], [width,0],[0,0]])
#         # 台形補正
#         trans_mat = cv2.getPerspectiveTransform(marker_coordinates, true_coordinates)
#         warped_img = cv2.warpPerspective(img, trans_mat, (width, height))
#         return warped_img
    
# 机の頂点座標を取得する関数
def getTabelPosition(img):
    # グレースケール変換
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    # ガウシアンブラーでノイズ除去
    blurred = cv2.GaussianBlur(gray, (7, 7), 0)
    # エッジ検出 (Canny) - 閾値調整
    edges = cv2.Canny(blurred, 50, 150)
    # 輪郭の検出
    contours, _ = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    # 最も大きな輪郭を探す (面積に基づくフィルタリング)
    max_area = 0
    screen_contour = None
    # 机の頂点座標を取得
    for contour in contours:
        area = cv2.contourArea(contour)
        if area > 1000:  # 面積が小さい輪郭は無視
            approx = cv2.approxPolyDP(contour, 0.02 * cv2.arcLength(contour, True), True)
            if len(approx) == 4 and area > max_area:  # 四角形かつ最大の面積
                screen_contour = approx
                max_area = area
    
    return screen_contour

# マーカーの座標を取得する関数
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

# 画像サイズを変更する関数
def scale_to_width(img, width):
    h, w = img.shape[:2]
    height = round(h * (width / w))
    dst = cv2.resize(img, dsize=(width, height))
    return dst