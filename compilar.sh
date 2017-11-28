echo "g++ src/prueba.cpp -o prueba -std=c++11 -I/home/pi/workspace/affectiva/include -I$HOME/workspace/plog/include/ -L$HOME/workspace/affectiva/lib -l affdex-native"
echo "..."
g++ src/prueba.cpp -o prueba -std=c++11 -I/home/pi/workspace/affectiva/include -I$HOME/workspace/plog/include/ -L$HOME/workspace/affectiva/lib -l affdex-native
echo "LISTO"
