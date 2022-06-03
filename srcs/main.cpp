#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstring>
#include <SFML/Graphics.hpp>

#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define WHITE   "\033[1;37m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define RESET   "\033[0m"

int max_iter = 128;
double zoom = 1.0;
double min_re = -2.5, max_re = 1;
double min_im = -1, max_im = 1;

bool error_msg(const std::string& s) {std::cerr << s << std::endl; return false;}

bool isBlank(const char& c) {return (c == ' ' || c == '\n' || c == '\t' || c == '\v' || c == '\f' || c == '\r') ? true : false;}

bool isDigit(const char& c) {return (c >= '0' && c <= '9') ? true : false;}

bool isInteger(const char* s) {for (int i = 0; i < strlen(s); i++) if (!isDigit(s[i])) return false; return true;}

void printMan() {
    std:: cout << WHITE << "MANUEL:" << RESET << std::endl;
    std::cout << "clic gauche = prend pour centre le pixel pointé et zoom *= 5" << std::endl;
    std::cout << "clic droit = zoom /= 5" << std::endl;
    std::cout << GREEN<<"Z" << RESET<<"/" << YELLOW << "X" RESET<<"= colors palett(" << GREEN<<"<" RESET<<"/" << YELLOW<<">" << RESET <<")" << std::endl;
    std::cout << BLUE<<"Q" << RESET << "/" << RED<<"E" << RESET<<" = zoom(" << BLUE<<"-" << RESET<<"/" << RED << "+" << RESET << ")" << std::endl;
    std::cout << BLUE<<"F" RESET<<"/" RED<<"R" << RESET<<"  = iteration(" << BLUE<<"-" << RESET<<"/" << RED << "+" << RESET<<")" << std::endl;
    std::cout << GREEN<<"A" << RESET<<"/" << YELLOW<<"D" << RESET"/" << RED<<"W" << RESET<<"/" << BLUE<<"S" << RESET<<" = move("<< GREEN<<"<" << RESET<<"/" << YELLOW<<">" << RESET<<"/" << RED<<"^" RESET<<"/" << BLUE<<"v" << RESET<<")" << std::endl;
    std::cout << std::endl;
}

sf::Color linear_interpolation(const sf::Color& v, const sf::Color& u, double a){
    auto const b = 1 - a;
    return sf::Color(b*v.r + a * u.r, b * v.g + a * u.g, b * v.b + a * u.b);
}

std::map<const std::string, std::vector<sf::Color> > loadColors() {
    std::map<const std::string, std::vector<sf::Color> >  colors;
    std::ifstream                       ifs("colorPalett", std::ios::in);
    if (!ifs) {std::cerr << "Error: can't open colorPalett." << std::endl; return colors;}
    std::string                         fileContent = "", s = "", nTmp = "", nTmp2;
    std::vector<sf::Color>              vTmp, vTmp2;
    sf::Color                           cTmp;
    getline(ifs, s);
    fileContent += s;
    while (getline(ifs, s)) fileContent += "\n" + s;
    ifs.close();
    if (fileContent.find(";") != std::string::npos) {std::cerr << "Error: colorPalett is not valid." << std::endl; return colors;}
    for (std::string::size_type f = fileContent.find("\n"); f != std::string::npos; f = fileContent.find("\n")) {fileContent.replace(f, 1, ".");}
    for (std::string::size_type f = fileContent.find(".."); f != std::string::npos; f = fileContent.find("..")) {fileContent.erase(f, 1);}
    for (std::string::size_type i = 0; i < fileContent.length() - 1; i++) {if (isDigit(fileContent[i]) && isBlank(fileContent[i + 1])) {fileContent.insert(i + 1, "."); i++;}}
    for (std::string::size_type i = 0; i < fileContent.length(); i++) {if (isBlank(fileContent[i])) {fileContent.erase(i, 1); i--;}}
    for (std::string::size_type f = fileContent.find(".,"); f != std::string::npos; f = fileContent.find(".,")) {fileContent.erase(f, 1);}
    for (std::string::size_type i = 0; i < fileContent.length() - 1; i++) if (!isDigit(fileContent[i]) && fileContent[i+1] == '.') {fileContent.erase(i + 1, 1); i++;}
    if (fileContent.empty()) {std::cerr << "Error: colorPalett is empty" << std::endl; return colors;} else fileContent += fileContent[fileContent.size() - 1] == '.' ? ";" : ".;";
    for (std::string::size_type i = 0; i < fileContent.length(); i++){
        if (i) i -= 1;
        if (isDigit(fileContent[i]) == true && fileContent[i] != ';') {std::cerr << "Error: colorPalett:0 c:" << i << "." << fileContent[i] << std::endl; colors.clear(); return colors;}
        if (fileContent[i] == ';') return colors;
        for (;!isDigit(fileContent[i]); i++) {nTmp += fileContent[i]; if (i == fileContent.length()){std::cerr << "Error: colorPalett:1 c:" << i << "." << std::endl; colors.clear(); return colors;}}
        if (nTmp.empty()) {std::cerr << "Error: colorPalett:2 c:" << i << "." << std::endl; colors.clear(); return colors;}
        for (int rTmp = 0, gTmp = 0, bTmp = 0, iTmp = 0;i < fileContent.length() && isDigit(fileContent[i]); i++) {
            for (char a = 0; a < 3; a++) {
                iTmp = 0;
                for (;i < fileContent.length() && isDigit(fileContent[i]); i++){
                    iTmp = iTmp * 10 + (fileContent[i] - '0');
                    if (iTmp > 255) {std::cerr << "Error: colorPalett:4 c:" << i << "." << std::endl; colors.clear(); return colors;}
                }
                if (i == s.length() || (a < 2 && fileContent[i] != ',') || (a == 2 && fileContent[i] != '.')) {std::cerr << "Error: colorPalett:5 c:" << i << "." << std::endl; colors.clear(); return colors;} else i++;
                if (a == 0) {rTmp = iTmp;} else if (a == 1) {gTmp = iTmp;} else {bTmp = iTmp;}
                if (a == 2) {i -= 1; cTmp = sf::Color(static_cast<sf::Uint8>(rTmp), static_cast<sf::Uint8>(gTmp), static_cast<sf::Uint8>(bTmp)); vTmp.push_back(cTmp);
                }
            }
        }
        vTmp2 = vTmp; nTmp2 = nTmp; colors.insert(std::make_pair(nTmp2, vTmp2)); nTmp.clear(); vTmp.clear();
    }
    return colors;
}

sf::Color calculMandelbrotPixel(std::vector<sf::Color> colors, int x, int y, int W, int H){
        double cr = min_re + (max_re - min_re) * x / W;
        double ci = min_im + (max_im - min_im) * y / H;
        double re = 0, im = 0;
        int iter;
        for (iter = 0; iter < max_iter; iter++){
            double tr = re * re - im * im + cr;
            im = 2 * re * im + ci;
            re = tr;
            if (re * re + im * im > 2 * 2) break;
        }
        static const auto max_color = colors.size() - 1;
        if (iter == max_iter) iter = 0;
        double mu = 1.0 * iter / max_iter;
        mu *= max_color;
        auto i_mu = static_cast<size_t>(mu);
        const sf::Color color1 = *(colors.begin() + i_mu);
        const sf::Color color2 = *(colors.begin() + std::min(i_mu + 1, max_color));
        return linear_interpolation(color1, color2, mu - i_mu);
}

#define NB_THREADS 1
void drawMandelbrot(sf::Image *image, std::vector<sf::Color> colors, const int& W, const int& H){
    int thread_id = 0, x, y;
    for (int i = 0; i < W * H; i++){
        if (thread_id == NB_THREADS) thread_id = 0;
        x = i % W; y = i / W;
        image->setPixel(x, y, calculMandelbrotPixel(colors, x, y, W, H));
        thread_id++;
    }
}

int main(int ac, char **av){
    if (ac != 2) {std::cerr << "Error: invalid number of arguments. Use \"./fractol <window's width>\"" << std::endl; return 1;}
    if (!isInteger(av[1])) {std::cerr << "Error: invalid width." << std::endl; return 1;}
    int W = std::atoi(av[1]), H = W/16*9;
    if (W < 16) {std::cerr << "Error: Window's width must be >= 16." << std::endl; return 1;}
    sf::RenderWindow    window(sf::VideoMode(W, H), "Mandelbrot");
    sf::Image image; image.create(W,H);
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Font font; font.loadFromFile("Helvetica.ttc");
    sf::Text text; text.setFont(font); text.setCharacterSize(24); text.setFillColor(sf::Color::Red);
    std::map<const std::string, std::vector<sf::Color> > colors = loadColors();
    if (colors.empty()) return 1;
std::cout << WHITE << "COLOR PALETTS:" << RESET << std::endl;
for (std::map<const std::string, std::vector<sf::Color> >::const_iterator it = colors.begin(); it != colors.end(); it++){
    std::cout << WHITE << it->first << ":" << RESET << std::endl;
    for (std::vector<sf::Color>::const_iterator it2 = it->second.begin(); it2 != it->second.end(); it2++)
        std::cout << RED << (int)it2->r << RESET << "," << GREEN << (int)it2->g << RESET << "," << BLUE << (int)it2->b << RESET << std::endl;
    std::cout << std::endl;
}
    std::map<const std::string, std::vector<sf::Color> >::const_iterator colorPalett = colors.begin();
    std::map<const std::string, std::vector<sf::Color> >::const_iterator colorPalettEnd = colors.end(); if (colors.size() > 1) colorPalettEnd--;

    printMan();
    while (window.isOpen()){
        sf::Event e;
        while (window.pollEvent(e)){
            if (e.type == sf::Event::Closed || (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)) window.close();
            if (e.type == sf::Event::KeyPressed){
                if (e.key.code == sf::Keyboard::A) {double w = (max_re - min_re) * 0.3; min_re -= w; max_re -= w;}
                if (e.key.code == sf::Keyboard::D) {double w = (max_re - min_re) * 0.3; min_re += w; max_re += w;}
                if (e.key.code == sf::Keyboard::W) {double h = (max_im - min_im) * 0.3; min_im -= h; max_im -= h;}
                if (e.key.code == sf::Keyboard::S) {double h = (max_im - min_im) * 0.3; min_im += h; max_im += h;}
                if (e.key.code == sf::Keyboard::F) {max_iter -= 10; if (max_iter < 1) max_iter = 1;}
                if (e.key.code == sf::Keyboard::R) {max_iter += 10; if (max_iter < 1) max_iter = 1;}
                if (e.key.code == sf::Keyboard::Q || e.key.code == sf::Keyboard::E){
                    auto zoom_x = [&](double z){
                        // mouse point will be new center point
                        double cr = min_re + (max_re - min_re) * (W/2) / W;
                        double ci = min_im + (max_im - min_im) * (H/2) / H;

                        // zoon
                        double tminr = cr - (max_re - min_re) / 2 / z;
                        max_re = cr + (max_re - min_re) / 2 / z;
                        min_re = tminr;

                        double tmini = ci - (max_im - min_im) / 2 / z;
                        max_im = ci + (max_im - min_im) / 2 / z;
                        min_im = tmini;
                    };
                    if (e.key.code == sf::Keyboard::Q) {zoom_x(1.0 / 2); zoom /= 2;}
                    if (e.key.code == sf::Keyboard::E) {zoom_x(2); ; zoom *= 2;}
                }
                if (e.key.code == sf::Keyboard::Z && colors.size() > 1) {
                    if (colorPalett == colors.begin()) 
                        colorPalett = colorPalettEnd; 
                    else
                        colorPalett--;
                }
                if (e.key.code == sf::Keyboard::X && colors.size() > 1) {
                    if (colorPalett == colorPalettEnd)
                        colorPalett = colors.begin();
                    else
                        colorPalett++;
                }
            }
            if (e.type == sf::Event::MouseButtonPressed){
                auto zoom_x = [&](double z){
                    // mouse point will be new center point
                    double cr = min_re + (max_re - min_re) * e.mouseButton.x / W;
                    double ci = min_im + (max_im - min_im) * e.mouseButton.y / H;

                    // zoon
                    double tminr = cr - (max_re - min_re) / 2 / z;
                    max_re = cr + (max_re - min_re) / 2 / z;
                    min_re = tminr;

                    double tmini = ci - (max_im - min_im) / 2 / z;
                    max_im = ci + (max_im - min_im) / 2 / z;
                    min_im = tmini;
                };
                if (e.mouseButton.button == sf::Mouse::Left) {zoom_x(5); ; zoom *= 5;}
                if (e.mouseButton.button == sf::Mouse::Right) {zoom_x(1.0 / 5); zoom /= 5;}
            }
        }
        window.clear();
        drawMandelbrot(&image, colorPalett->second, W, H);
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        window.draw(sprite);
        char str[100];
        sprintf(str, "max_iter: %d\nzoom:x%2.2lf", max_iter, zoom);
        text.setString(str);
        window.draw(text);
        window.display();
    }
}