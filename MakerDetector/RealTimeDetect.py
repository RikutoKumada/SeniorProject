import cv2
import numpy as np
import customFunctions as cf

# 固定値
ROBOT_MARKER_ID = 0
TARGET_MARKER_ID = 1
ISDISPLAY = True # 画面表示
ISCAPTURE = False # 動画保存
ISKEYSTONE = False # 台形補正

# 机の四隅の座標（初期化時に取得）
tablePos = None

# カメラの起動
cap = cv2.VideoCapture(0)
# 初期フレームの取得
ret, frame = cap.read()
if ret:
    # 机の頂点座標を取得
    tablePos = cf.getTabelPosition(frame)

if tablePos is not None:
    width, height = (1280, 1280) # 変形後画像サイズ
else:
    width, height = (int(cap.get(3)), int(cap.get(4)))

# 動画保存のための設定
if ISCAPTURE:
    fourcc = cv2.VideoWriter_fourcc(*'XVID')
    out = cv2.VideoWriter('videos/Result.avi', fourcc, 30, (width, height))

marker_num = 0

while True:
    # フレームの取得
    ret, frame = cap.read()
    if not ret:
        break

    # 台形補正
    if ISKEYSTONE and tablePos is not None:
        # 頂点の用意
        marker_coordinates = np.float32(tablePos)
        true_coordinates   = np.float32([[0,height], [width,height], [width,0],[0,0]])
        # 台形補正
        trans_mat = cv2.getPerspectiveTransform(marker_coordinates, true_coordinates)
        frame = cv2.warpPerspective(frame, trans_mat, (width, height))

    # マーカーの座標を取得
    marker_pos = cf.getMarkerPositions(frame, [ROBOT_MARKER_ID, TARGET_MARKER_ID])

    if(marker_pos):
        marker_num += 1
        for pos_data in marker_pos:
            x = pos_data[1][0]
            y = pos_data[1][1]
            print(str(marker_num) + "回目の検出 | x:" + str(x) + " y:" +str(y))

    for pos_data in marker_pos:
        cv2.circle(frame, (int(pos_data[1][0]), int(pos_data[1][1])), 5, (0,0,255), -1)

    # 結果のフレームを表示
    if ISDISPLAY:
        cv2.imshow('Video', frame)
    # 録画用にフレームを書き込み
    if ISCAPTURE:
        out.write(frame)

    # 'q'キーを押すと終了
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# カメラ、ビデオ、ウィンドウの解放
cap.release()
if ISDISPLAY:
    cv2.destroyAllWindows()
if ISCAPTURE:
    out.release()