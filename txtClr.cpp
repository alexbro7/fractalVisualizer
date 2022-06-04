#include <iostream>

#define RESET       "\033[0m"
#define BLACK       "\033[1;30m"
#define RED         "\033[1;31m"
#define GREEN       "\033[1;32m"
#define YELLOW      "\033[1;33m"
#define BLUE        "\033[1;34m"
#define MAGENTA     "\033[1;35m"
#define CYAN        "\033[1;36m"
#define WHITE       "\033[1;37m"
#define WRED        "\033[1;41m"
#define WGREEN      "\033[1;42m"
#define WYELLOW     "\033[1;43m"
#define WBLUE       "\033[1;44m"
#define WMAGENTA    "\033[1;45m"
#define WCYAN       "\033[1;46m"
#define GWHITE      "\033[1;47m"

#define A "\033[1m"
#define B "\033[4m"
#define C "\033[7m"
#define D "\033[0m"
#define E "\033[5m"

int main(){
	std::cout << "Voici le \033[33m\033[1m\033[4m\033[41mle \033[7mtexte\033[0m mis en forme.\n";
	std::cout << "Voici le " YELLOW A B WRED "le " C "texte" D " mis en forme.\n";
	std::cout << "@012468wXyZ:.\n";
	std::cout << RED A "@012468wXyZ:.\n";
	std::cout << GREEN B "@012468:.\n";
	std::cout << YELLOW C "@012468:.\n";
	std::cout << BLUE D "@012468:.\n";
	std::cout << PINK E "@012468:.\n";
	return 0;
}
