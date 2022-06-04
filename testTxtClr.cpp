#include <iostream>

#define RESET       "\033[0m"
#define BLACK       "\033[30m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define PINK	    "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"
#define WRED        "\033[41m"
#define WGREEN      "\033[42m"
#define WYELLOW     "\033[43m"
#define WBLUE       "\033[44m"
#define WPINK    	"\033[45m"
#define WCYAN       "\033[46m"
#define GWHITE      "\033[47m"

#define A "\033[1m"
#define B "\033[4m"
#define C "\033[7m"
#define D "\033[0m"
#define E "\033[5m"

int main(){
	std::cout << "Voici le \033[33m\033[1m\033[4m\033[41mle \033[7mtexte\033[0m mis en forme.\n";
	std::cout << "Voici le " YELLOW A B WRED "le " C "texte" D " mis en forme.\n";
	std::cout << D RED << "DEFAULT" << "\t\t\t@012468wXyZ:.\n";
	std::cout << D;
	std::cout << D PINK << "?" << "\t\t\t@012468:.\n";
	std::cout << D;
	std::cout << A GREEN << "GRAS" << "\t\t\t@012468wXyZ:.\n";
	std::cout << D;
	std::cout << B YELLOW << "SOUS_LIGNE" << "\t\t\t@012468:.\n";
	std::cout << D;
	std::cout << C BLUE << "BACKGROUND" << "\t\t\t@012468:.\n";
	std::cout << D;
	std::cout << E CYAN << "CLIGNOTANT" << "\t\t\t@012468:.\n";
	std::cout << D;
	return 0;
}
