#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstring>
#include <SFML/Graphics.hpp>
#include <thread>

//░▒▓███████████████▓▒░
//░▒▓█ TEXT COLORS █▓▒░
//░▒▓███████████████▓▒░
#define CN      "\033[30m" // color_gray
#define CR      "\033[31m" // color_red
#define CG      "\033[32m" // color_green
#define CB      "\033[34m" // color_blue
#define CP      "\033[35m" // color_magenta
#define CY      "\033[33m" // color_yellow
#define CC      "\033[36m" // color_cyan
#define CW      "\033[37m" // color_white
#define ON      "\033[40m" // overline_gray
#define OR      "\033[41m" // overline_red
#define OG      "\033[42m" // overline_green
#define OB      "\033[44m" // overline_blue
#define OP      "\033[45m" // overline_magenta
#define OY      "\033[43m" // overline_yellow
#define OC      "\033[46m" // overline_cyan
#define OW      "\033[47m" // overline_white
//░▒▓██████████████▓▒░
//░▒▓ TEXT EFFECTS ▓▒░
//░▒▓██████████████▓▒░
#define BLD	    "\033[1m"  // bold 
#define UL      "\033[4m"  // underline
#define FLH     "\033[5m"  // bold 
#define OL      "\033[7m"  // overline
#define RST	    "\033[0m"  // effect/color reset


/*░▒▓█████████▓▒░
  ░▒▓█ UTILS █▓▒░
  ░▒▓█████████▓▒░*/
bool    error_msg(const std::string& s) {std::cerr << s << std::endl; return false;}
bool    isBlank(const char& c) {return (c == ' ' || c == '\n' || c == '\t' || c == '\v' || c == '\f' || c == '\r') ? true : false;}
bool    isDigit(const char& c) {return (c >= '0' && c <= '9') ? true : false;}
bool    isInteger(const char* s) {for (std::string::size_type i = 0; i < strlen(s); i++) if (!isDigit(s[i])) return false; return true;}
int     integerLength(int i) {if (!i) return 1; for (int r = 0; 1; r++) {if (!i) return r; else i /= 10;}}
//【 】〔〕 〖〗 〚〛
/*
░▒▓████████████████████▓▒░
░▒▓█ STRING GENERATOR █▓▒░
░▒▓████████████████████▓▒░*/
const std::string    displayManual(void) {
    std::string r = (RST CW "░▒▓" OW CN "CONTROLS" RST UL CW "▓▒░" RST "\n\n");

    r += OR CW "[R]";
    r += RST CW "|";
    r += OC CW "[F]";
    r += RST CW " =" RST " iteration" BLD CR "+" CC "-" CW "1\n"; r+= RST "\n";

    r += OR CW "[E]";
    r += RST CW "|";
    r += OC CW "[Q]";
    r += RST CW " =" RST " Zoom" BLD CW"*" CR "2" CC "1/2\n"; r+= RST "\n";

    r += OR CW "[W]";
    r += RST CW "|";
    r += OC CW "[S]";
    r += RST CW "|";
    r += OG CW "[D]";
    r += RST CW "|";
    r += OP CW "[A]";
    r += RST CW " = View origin ";r+= CG CC "↑";r += CW "|";r += CR "↓";r += CW "|";r+= CG "→";r += CW "|";r += CP "←"; r+= RST "\n\n";

    r += RST CW "░▒▓████████▓▒░\n";r+= RST;

    return r;
}

const std::string vecClrToStr(const std::string& n, const std::vector<sf::Color>& v){
    std::string s(BLD ON CW " "); s += n + ":"; for (std::string::size_type a = 18; a > n.length(); a--) s += " "; s+= RST "\n";
    s += ON;
    for (std::string::size_type a = 20; a > 0; a--) s += " "; s += RST "\n";
    s += BLD ON CW;// s+= ON CN BLD;
    for (std::vector<sf::Color>::const_iterator it = v.begin(); it != v.end(); it++){
        s += ON " " OR;
        for (char a = 3; a > integerLength((int)it->r); a--) s += " "; s += " ";
        s += " "; s += std::to_string((int)it->r) + " " ON; s += " " OG;
        for (char a = 3; a > integerLength((int)it->g); a--) s += " "; s += " ";
        s += std::to_string((int)it->g) + " " ON; s += " " OB;
        for (char a = 3; a > integerLength((int)it->b); a--) s += " "; s += " ";
        s += std::to_string((int)it->b) + " " ON " "; s += RST"\n";
        s += BLD ON CW;
    }
    for (std::string::size_type a = 20; a > 0; a--) s += " "; s += RST "\n";
    return s;
}


sf::Color linear_interpolation(const sf::Color& v, const sf::Color& u, double a) {auto const b = 1 - a; return sf::Color(b*v.r + a * u.r, b * v.g + a * u.g, b * v.b + a * u.b);}

std::map<const std::string, std::vector<sf::Color> > loadColors() {
    std::map<const std::string, std::vector<sf::Color> >  colors;
    std::ifstream                       ifs("colorPalett", std::ios::in);
    if (!ifs) {std::cerr << "Error: can't open colorPalett." << std::endl; return colors;}
    if (!ifs) {std::cerr << "Error: can't open colorPalett." << std::endl; return colors;}
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

//░▒▓█|A DEPLACÉ|█▓▒░
int max_iter = 128;
double zoom = 1.0;
double min_re = -2.5, max_re = 1;
double min_im = -1, max_im = 1;

sf::Color calculMandelbrotPixel(std::vector<sf::Color> colors, int x, int y, int windowWidth, int windowHeight){
        double cr = min_re + (max_re - min_re) * x / windowWidth;
        double ci = min_im + (max_im - min_im) * y / windowHeight;
        double re = 0, im = 0;
        int iter;
        for (iter = 0; iter < max_iter; iter++){
            double tr = re * re - im * im + cr;
            im = 2 * re * im + ci;
            re = tr;
            if (re * re + im * im > 2 * 2) break;
        }
        // int r = 1.0 * (max_iter - iter) / max_iter * 0xff;
        // int g = r, b = r;


        static const auto max_color = colors.size() - 1;
        if (iter == max_iter) iter = 0;
        double mu = 1.0 * iter / max_iter;
        //scale mu to be in the range of colors
        mu *= max_color;
        auto i_mu = static_cast<size_t>(mu);
        const sf::Color color1 = colors[i_mu];
        const sf::Color color2 = colors[std::min(i_mu + 1, max_color)];
        return linear_interpolation(color1, color2, mu - i_mu);
}

// #define NB_THREADS 1
// void draw(sf::Image *image, std::string colorsName, std::vector<sf::Color> colors, const int& windowWidth, const int& windowHeight){
//     int thread_id = 0, x, y;
//     for (int i = 0; i < windowWidth * windowHeight; i++){
//         if (thread_id == NB_THREADS) thread_id = 0;
//         x = i % windowWidth; y = i / windowWidth;
//         image->setPixel(x, y, calculMandelbrotPixel(colors, x, y, windowWidth, windowHeight));
//         thread_id++;
//     }
// }
#define NB_THREADS 8

void threading(int part, int windowWidth, int windowHeight, std::vector<sf::Color> colors, sf::Image &image)
{
    int x, y;
    int i = (windowWidth * windowHeight) / NB_THREADS * part;  // the start pixel of the current thread
    int end = ((windowWidth * windowHeight) / NB_THREADS) + i; // the end pixel of the current thread
    for (; (i < windowWidth * windowHeight) && (i < end); i++) // while max pixel, or max pixel in this thread
    {
        x = i % windowWidth;
        y = i / windowWidth;
        sf::Color c = calculMandelbrotPixel(colors, x, y, windowWidth, windowHeight);
        image.setPixel(x, y, c);
    }
}

void draw(sf::Image *image, std::vector<sf::Color> colors, const int &windowWidth, const int &windowHeight)
{
    std::thread t[windowWidth * windowHeight];
    // int thread_id = 0;
    for (int i = 0; i < NB_THREADS; i++) // create thread for each part of the image (nb of thread)
    {
        t[i] = std::thread(threading, i, windowWidth, windowHeight, colors, std::ref(*image));
    }
    for (int i = 0; i < NB_THREADS; i++) // wait for all threads to finish
    {
        t[i].join();
    }
}
/**/

int main(int ac, char **av){
    // if (ac != 2) {std::cerr << "Error: invalid number of arguments. Use \"./fractol <window's width>\"" << std::endl; return 1;}
    // if (!isInteger(av[1])) {std::cerr << "Error: invalid width." << std::endl; return 1;}
    // int windowWidth = std::atoi(av[1]), windowHeight = windowWidth/16*9;
    // if (windowWidth < 16) {std::cerr << "Error: Window's width must be >= 16." << std::endl; return 1;}
    // sf::RenderWindow    window(sf::VideoMode(windowWidth, windowHeight), "Mandelbrot"); window.setFramerateLimit(60); 
    // sf::Image image; image.create(windowWidth,windowHeight);
    // sf::Texture texture;
    // sf::Sprite sprite;
    // sf::Font font; font.loadFromFile("Helvetica.ttc");
    // sf::Text text; text.setFont(font); text.setCharacterSize(24); text.setFillColor(sf::Color::Red);
    // std::map<const std::string, std::vector<sf::Color> > colors = loadColors();
    // if (colors.empty()) return 1;
    // std::map<const std::string, std::vector<sf::Color> >::const_iterator colorPalett = colors.begin();
    // std::map<const std::string, std::vector<sf::Color> >::const_iterator colorPalettEnd = colors.end(); if (colors.size() > 1) colorPalettEnd--;
std::cout << displayManual();return 0;
//     while (window.isOpen()){
//         sf::Event e;
//         while (window.pollEvent(e)){
//             if (e.type == sf::Event::Closed || (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)) window.close();
//             if (e.type == sf::Event::KeyPressed){
//                 if (e.key.code == sf::Keyboard::A) {double w = (max_re - min_re) * 0.3; min_re -= w; max_re -= w;}
//                 if (e.key.code == sf::Keyboard::D) {double w = (max_re - min_re) * 0.3; min_re += w; max_re += w;}
//                 if (e.key.code == sf::Keyboard::W) {double h = (max_im - min_im) * 0.3; min_im -= h; max_im -= h;}
//                 if (e.key.code == sf::Keyboard::S) {double h = (max_im - min_im) * 0.3; min_im += h; max_im += h;}
//                 if (e.key.code == sf::Keyboard::F) {max_iter -= 10; if (max_iter < 1) max_iter = 1;}
//                 if (e.key.code == sf::Keyboard::R) {max_iter += 10; if (max_iter < 1) max_iter = 1;}
//                 if (e.key.code == sf::Keyboard::Q || e.key.code == sf::Keyboard::E){
//                     auto zoom_x = [&](double z){
//                         // mouse point will be new center point
//                         double cr = min_re + (max_re - min_re) * (windowWidth/2) / windowWidth;
//                         double ci = min_im + (max_im - min_im) * (windowHeight/2) / windowHeight;

//                         // zoon
//                         double tminr = cr - (max_re - min_re) / 2 / z;
//                         max_re = cr + (max_re - min_re) / 2 / z;
//                         min_re = tminr;

//                         double tmini = ci - (max_im - min_im) / 2 / z;
//                         max_im = ci + (max_im - min_im) / 2 / z;
//                         min_im = tmini;
//                     };
//                     if (e.key.code == sf::Keyboard::Q) {zoom_x(1.0 / 2); zoom /= 2;}
//                     if (e.key.code == sf::Keyboard::E) {zoom_x(2); ; zoom *= 2;}
//                 }
//                 if (e.key.code == sf::Keyboard::M) displayManual(); std::cout << std::endl;
//                 if (e.key.code == sf::Keyboard::Z && colors.size() > 1) {
//                     if (colorPalett == colors.begin()) 
//                         colorPalett = colorPalettEnd; 
//                     else
//                         colorPalett--;
//                     std::cout << vecClrToStr(colorPalett->first, colorPalett->second) << std::endl;
//                 }
//                 if (e.key.code == sf::Keyboard::X && colors.size() > 1) {
//                     if (colorPalett == colorPalettEnd)
//                         colorPalett = colors.begin();
//                     else
//                         colorPalett++;
//                     std::cout << vecClrToStr(colorPalett->first, colorPalett->second) << std::endl;
//                 }
//             }
//             if (e.type == sf::Event::MouseButtonPressed){
//                 auto zoom_x = [&](double z){
//                     // mouse point will be new center point
//                     double cr = min_re + (max_re - min_re) * e.mouseButton.x / windowWidth;
//                     double ci = min_im + (max_im - min_im) * e.mouseButton.y / windowHeight;

//                     // zoon
//                     double tminr = cr - (max_re - min_re) / 2 / z;
//                     max_re = cr + (max_re - min_re) / 2 / z;
//                     min_re = tminr;

//                     double tmini = ci - (max_im - min_im) / 2 / z;
//                     max_im = ci + (max_im - min_im) / 2 / z;
//                     min_im = tmini;
//                 };
//                 if (e.mouseButton.button == sf::Mouse::Left) {zoom_x(5); ; zoom *= 5;}
//                 if (e.mouseButton.button == sf::Mouse::Right) {zoom_x(1.0 / 5); zoom /= 5;}
//             }
//         }
//         window.clear();
//         draw(&image, colorPalett->second, windowWidth, windowHeight);
//         texture.loadFromImage(image);
//         sprite.setTexture(texture);
//         window.draw(sprite);
//         char str[100];
//         sprintf(str, "max_iter: %d\nzoom:x%2.2lf", max_iter, zoom);
//         // text.setString(str);
//         // window.draw(text);
//         window.display();
//     }
//     return 0;
}