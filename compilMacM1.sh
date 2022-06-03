
#  clang++ srcs/main.cpp -I/opt/homebrew/Cellar/sfml/2.5.1_1/include/  
#  clang++ srcs/main.cpp -std=c++11 -I/opt/homebrew/Cellar/sfml/2.5.1_1/include/
# clang++ srcs/main.cpp -std=c++11 -I/opt/homebrew/Cellar/sfml/2.5.1_1/include/ -lsfml-graphics -lsfml-window -lsfml-system
clang++ srcs/main.cpp -std=c++11 -I/opt/homebrew/Cellar/sfml/2.5.1_1/include/ -L/opt/homebrew/Cellar/sfml/2.5.1_1/lib -lsfml-graphics -lsfml-window -lsfml-system