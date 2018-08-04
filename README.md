# as_ntu_find_clouds
從魚眼鏡頭的照片辨識雲

as_ntu_find_clouds [參數] <input_image_file>

參數說明如下：
input_image_file : 輸入的魚眼鏡頭照片(jpg file)。
-rotate : 照片計算時旋轉的度數(順時針旋轉)。預設：0度。
-region : 照片計算時，取樣的範圍度數(從天頂開始計算的)。預設：60度。
-out_image : 經過繪圖著色，輸出的照片(png file)。無設定時忽略。預設：忽略。
-rb : 顏色R/B的比值超過多少時，視為雲。預設：0.8。
-gb : 顏色G/B的比值超過多少時，視為雲。預設：0.8。
-latitude : 指定緯度(-90~90)，忽略照片中讀取出的值。預設：使用照片中的值。
-longitude : 指定經度(-180~180)，忽略照片中讀取出的值。預設：使用照片中的值。
-sun_region : 太陽遮罩的範圍度數。預設：5度。
-ignore_sun : 忽略太陽遮罩區域的計算。預設：不忽略。

sample:
./as_ntu_find_clouds -rotate -9 -out_image ../../findclouds/output/test.png ../../findclouds/sample/20180529235500_12.jpg
./as_ntu_find_clouds -rotate 10.3 -region 55.5 -rb 0.7 -gb 0.75 ../../findclouds/sample/20180529235500_12.jpg
./as_ntu_find_clouds -ignore_sun -out_image abc.png ../../findclouds/sample/20180529235500_12.jpg
