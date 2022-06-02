rm main.o && rm sfml-app
g++ -c srcs/main.cpp -I include/ 
g++ main.o -o sfml-app -L lib/ -lsfml-graphics -lsfml-window -lsfml-system
export LD_LIBRARY_PATH=lib/ && ./sfml-app 960