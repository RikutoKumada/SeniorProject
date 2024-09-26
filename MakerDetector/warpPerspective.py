import cv2
import numpy as np

def warpPerspective(img, img_size):
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

    if screen_contour is None:
        print("机の輪郭が見つかりませんでした")
        return None
    else:
        # 変形後画像サイズ
        width, height = img_size
        # 頂点の用意
        marker_coordinates = np.float32(screen_contour)
        true_coordinates   = np.float32([[0,height], [width,height], [width,0],[0,0]])
        # 台形補正
        trans_mat = cv2.getPerspectiveTransform(marker_coordinates, true_coordinates)
        warped_img = cv2.warpPerspective(img, trans_mat, (width, height))
        return warped_img


# 画像の読み込み
img = cv2.imread('videos/screenshot.png')

warped = warpPerspective(img, (1000,1000))



# if warped is not None:
#     # 画像の保存
#     cv2.imwrite("videos/warped.png", warped)
#     # 結果の表示
#     while True:
#         cv2.imshow("Warped", warped)
#         key = cv2.waitKey(10)
#         if key == 27:  # Escキーで終了
#             break
#     # ウィンドウの削除
#     cv2.destroyAllWindows()
