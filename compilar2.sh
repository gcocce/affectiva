echo "compilando"
echo "..."
g++ src/prueba.cpp -o prueba -std=c++11 -I/home/pi/workspace/affectiva/include -I$HOME/workspace/plog/include -L$HOME/workspace/affectiva/lib -laffdex-native -I/usr/local/include/opencv -I/usr/local/include -I/usr/local/include/opencv2 -L/usr/local/lib -lopencv_ml -lopencv_objdetect -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_core
echo "LISTO"
