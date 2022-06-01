#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

const int W = 960;
const int H = 540;

int max_iter = 128;
double zoom = 1.0;
double min_re = -2.5, max_re = 1;
double min_im = -1, max_im = 1;

bool error_msg(const std::string& s) {std::cerr << s << std::endl; return false;}

sf::Color linear_interpolation(const sf::Color& v, const sf::Color& u, double a){
    auto const b = 1 - a;
    return sf::Color(b*v.r + a * u.r, b * v.g + a * u.g, b * v.b + a * u.b);
}

void drawMandelbrot(sf::Image *image, const std::vector<sf::Color>& colors){
    for (int y = 0; y < H; y++) for (int x = 0; x < W; x++){
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
        //scale mu to be in the range of colors
        mu *= max_color;
        auto i_mu = static_cast<size_t>(mu);
        const sf::Color color1 = colors[i_mu];
        const sf::Color color2 = colors[std::min(i_mu + 1, max_color)];
        sf::Color c = linear_interpolation(color1, color2, mu - i_mu);
        image->setPixel(x, y, sf::Color(c));
    }
}

// int main2(void){
//     sf::RenderWindow    window(sf::VideoMode(W, H), "Mandelbrot");
//     sf::Image image; image.create(W,H);
//     sf::Texture texture;
//     sf::Sprite sprite;
//     sf::Font font; font.loadFromFile("Helvetica.ttc");
//     sf::Text text; text.setFont(font); text.setCharacterSize(24); text.setFillColor(sf::Color::Red);
//     std::vector<sf::Color> colors;
//     colors.push_back(sf::Color(0,0,0));
//     // colors.push_back(sf::Color(232,37,73));
//     // colors.push_back(sf::Color(237,55,55));
//     // colors.push_back(sf::Color(55,170,200));
//     // colors.push_back(sf::Color(150,2,150));
//     colors.push_back(sf::Color(213,67,31));
//     colors.push_back(sf::Color(251,255,121));
//     colors.push_back(sf::Color(62,223,89));
//     colors.push_back(sf::Color(43,30,218));
//     colors.push_back(sf::Color(0,255,247));
//     while (window.isOpen()){
//         sf::Event e;
//         while (window.pollEvent(e)){
//             if (e.type == sf::Event::Closed || (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)) window.close();
//             if (e.type == sf::Event::KeyPressed){
//                 // std::cout << "*" << e.key.code << "*" << std::endl;
//                 if (e.key.code == sf::Keyboard::A) {double w = (max_re - min_re) * 0.3; min_re -= w; max_re -= w;}
//                 if (e.key.code == sf::Keyboard::D) {double w = (max_re - min_re) * 0.3; min_re += w; max_re += w;}
//                 if (e.key.code == sf::Keyboard::W) {double h = (max_im - min_im) * 0.3; min_im -= h; max_im -= h;}
//                 if (e.key.code == sf::Keyboard::S) {double h = (max_im - min_im) * 0.3; min_im += h; max_im += h;}
//                 if (e.key.code == sf::Keyboard::F) {max_iter -= 10; if (max_iter < 1) max_iter = 1;}
//                 if (e.key.code == sf::Keyboard::R) {max_iter += 10; if (max_iter < 1) max_iter = 1;}
//                 if (e.key.code == sf::Keyboard::Q || e.key.code == sf::Keyboard::E){
//                     auto zoom_x = [&](double z){
//                         // mouse point will be new center point
//                         double cr = min_re + (max_re - min_re) * (W/2) / W;
//                         double ci = min_im + (max_im - min_im) * (H/2) / H;

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
//             }
//             // if (e.type == sf::Event::MouseWheelScrolled){
//                 // if (e.MouseWheelScrolled){
//                     // if (e.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel){
//                         // if (e.mouseWheelScroll.delta > 0) max_iter *= 2;
//                         // else max_iter /= 2;
//                         // if (max_iter < 1) max_iter = 1;
//                     // }
//                 // }
//             // }
//             if (e.type == sf::Event::MouseButtonPressed){
//                 auto zoom_x = [&](double z){
//                     // mouse point will be new center point
//                     double cr = min_re + (max_re - min_re) * e.mouseButton.x / W;
//                     double ci = min_im + (max_im - min_im) * e.mouseButton.y / H;

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
//         drawMandelbrot(&image, colors);
//         texture.loadFromImage(image);
//         sprite.setTexture(texture);
//         window.draw(sprite);
        
//         char str[100];
//         sprintf(str, "max_iter: %d\nzoom:x%2.2lf", max_iter, zoom);
//         text.setString(str);
//         window.draw(text);

//         window.display();
//     }
// }


// bool readColor(sf::Color *colors, const std::string& fileContent, std::string::size_type *i, std::string::size_type *l){
//     int rTmp = 0, gTmp = 0, bTmp = 0;
//     //pass characteres blancs.
//     for (;*i < fileContent.length() && isBlank(fileContent[*i]); *i += 1) if (fileContent[*i] == '\n') *l += 1;
//     // recupere rouge.
//     for (;*i < fileContent.length() && std::isdigit(fileContent[*i]); *i += 1) {rTmp *= 10; rTmp += fileContent[*i] - '0'; if (rTmp > 255) {std::cout << "errA" << std::endl; return 1;}}
//     // pass characteres blancs.
//     for (;*i < fileContent.length() && isBlank(fileContent[*i]); *i += 1) if (fileContent[*i] == '\n') *l += 1;
//     // pass ','
//     if (*i == fileContent.length() || fileContent[*i] != ',') {std::cout << "errB" << std::endl; return 1;} else *i += 1;
//     for (;*i < fileContent.length() && isBlank(fileContent[*i]); *i += 1) if (fileContent[*i] == '\n') *l +=1;
//     if (*i == fileContent.length() || !std::isdigit(fileContent[*i])) {std::cout << "errC" << std::endl; return 1;}
//     for (;*i < fileContent.length() && std::isdigit(fileContent[*i]); *i += 1) {gTmp *= 10; gTmp += fileContent[*i] - '0'; if (gTmp > 255) {std::cout << "errD" << std::endl; return 1;}}
//     for (;*i < fileContent.length() && isBlank(fileContent[*i]); *i += 1) if (fileContent[*i] == '\n') *l += 1;
//     if (*i == fileContent.length() || fileContent[*i] != ',') {std::cout << "errE" << std::endl; return 1;} else *i += 1;
//     for (;*i < fileContent.length() && isBlank(fileContent[*i]); *i += 1) if (fileContent[*i] == '\n') *l += 1;
//     if (*i == fileContent.length() || !std::isdigit(fileContent[*i])) {std::cout << "errF" << std::endl; return 1;}
//     for (;*i < fileContent.length() && std::isdigit(fileContent[*i]); *i += 1) {bTmp *= 10; bTmp += fileContent[*i] - '0'; if (bTmp > 255) {std::cout << "errG" << std::endl; return 1;}}
//     *colors = sf::Color(static_cast<sf::Uint8>(rTmp), static_cast<sf::Uint8>(gTmp), static_cast<sf::Uint8>(bTmp));
//     return 0;
// }


// bool norm_colorPalett(const std::string& s){
//     if (s.empty()) {std::cerr << "Error: colorPalett is empty" << std::endl; return 1;}
//     std::string::size_type i = 0;
//     while ()
// }

bool is_blank(char c) {return (c == ' ' || c == '\n' || c == '\t' || c == '\v' || c == '\f' || c == '\r') ? true : false;}

bool is_digit(char c) {return (c >= '0' && c <= '9') ? true : false;}

std::map<const std::string, std::vector<sf::Color> > loadColors(){
    std::map<const std::string, std::vector<sf::Color> >  colors;
    std::ifstream                       ifs("colorPalett", std::ios::in);
    if (!ifs) {std::cerr << "Error: can't colorPalett." << std::endl; return colors;}
    std::string                         fileContent = "", s = "", nTmp = "";
    std::vector<sf::Color>              vTmp;
    sf::Color                           cTmp;

    getline(ifs, s);
    fileContent += s;
    while (getline(ifs, s)) fileContent += "\n" + s;
    ifs.close();
    std::cout << "---------------------------------------------------------------------------------------------------------------" << std::endl << fileContent << std::endl;
    for (std::string::size_type f = fileContent.find("\n"); f != std::string::npos; f = fileContent.find("\n")) fileContent.replace(f, 1, ":");
    for (std::string::size_type f = fileContent.find(":"); f != std::string::npos; f = fileContent.find(":")) fileContent.erase(f, 1);
    std::cout << "---------------------------------------------------------------------------------------------------------------" << std::endl << fileContent << std::endl;
    std::cout << "OK" << std::endl;
    for (std::string::size_type i = 0; i < fileContent.length() - 1; i++) {
        if (is_digit(s[i]) && is_blank(s[i + 1])) {
            fileContent.insert(i, "%");
            i++;
        }
    }
    std::cout << "---------------------------------------------------------------------------------------------------------------" << std::endl << fileContent << std::endl;
    // for (std::string::size_type i = 0; i < fileContent.length() - 1; i++) {if (isBlank(s[i])) {fileContent.erase(i, 1);}}
    // std::cout << "---------------------------------------------------------------------------------------------------------------" << std::endl << fileContent << std::endl;
    return colors;











//    while(fileContent.find(" ") != std::string::npos || fileContent.find("\t") != std::string::npos || fileContent.find("\v") != std::string::npos || fileContent.find("\f") != std::string::npos || fileContent.find("\r") != std::string::npos)
//        for (std::string::size_type i = 0; i < fileContent.length(); i++) if (isblank(fileContent[i])) fileContent.erase(i, 1);
//    for (std::string::size_type i = 0; i < fileContent.length(); i++) if (i && (isdigitfileContent[i - 1]
//    std::string::size_type i = 0, j = 0, l = 0;
//     while (i < fileContent.length() && (isBlank(fileContent[i]) || s[i] == '\n')) {if (fileContent[i] == '\n') l++; i++;}
//     for (; i < fileContent.length(); i++){
//         if (std::isdigit(fileContent[i])) {std::cerr << "Error1: colorPalett l:" << l << "." << std::endl; colors.clear(); return colors;}
//         j = i;
//         while (i < fileContent.length() && (!isBlank(fileContent[i]) && s[i] != '\n')) i++;
//         nTmp = fileContent.substr(j, i - j);
//         while (i < fileContent.length() && (isBlank(fileContent[i]) || s[i] == '\n')) {if (fileContent[i] == '\n') l++; i++;}
//         if (!nTmp.length() && i < fileContent.length()) {std::cerr << "Error4: colorPalett l:" << l << "." << std::endl; colors.clear(); return colors;}
// std::cout << nTmp << std::endl;
//         while (i < fileContent.length() && std::isdigit(fileContent[i])){
//             if (readColor(&cTmp, fileContent, &i, &l)) {std::cerr << "Error3: colorPalett l:" << l << "." << std::endl; colors.clear(); return colors;}
//             vTmp.push_back(cTmp);
//         }
//         if (nTmp.length() && vTmp.empty()) {std::cerr << "Error2: colorPalett  l:" << l << "." << std::endl; colors.clear(); return colors;}
//         if (vTmp.empty()) {std::cerr << "Error: colorPalett is empty." << std::endl; return colors;}
//         colors.insert(std::make_pair(nTmp, vTmp)); nTmp = ""; vTmp.clear();
//         while (i < fileContent.length() && (isBlank(fileContent[i]) || s[i] == '\n')) {if (fileContent[i] == '\n') l++; i++;}
//     }
}

int main(void){
    std::map<const std::string, std::vector<sf::Color> >colors = loadColors();
    return 0;
    // sf::RenderWindow    window(sf::VideoMode(W, H), "Fractol");
    // sf::Image image; image.create(500,500);
    // sf::Texture texture;
    // sf::Sprite sprite;
    // sf::Font font; font.loadFromFile("Helvetica.ttc");
    // sf::Text text; text.setFont(font); text.setCharacterSize(24); text.setFillColor(sf::Color::White);
    // std::vector<std::vector<sf::Color> >colors = loadColors();
    // while (window.isOpen()){
    //     sf::Event e;
    //     while (window.pollEvent(e)){
    //         if (e.type == sf::Event::Closed) window.close();
    //         if (e.type == sf::Event::KeyPressed){
    //             if (e.key.code == sf::Keyboard::Escape) window.close();
    //             }
    //         }
    //         if (e.type == sf::Event::MouseButtonPressed){
    //                 // double cr = min_re + (max_re - min_re) * e.mouseButton.x / W;
    //                 // double ci = min_im + (max_im - min_im) * e.mouseButton.y / H;
    //             if (e.mouseButton.button == sf::Mouse::Left) {

    //             }
    //             // if (e.mouseButton.button == sf::Mouse::Right) {}
    //         }
    //     }
    //     window.clear();
    //     drawMandelbrot(&image, colors);
    //     texture.loadFromImage(image);
    //     sprite.setTexture(texture);
    //     window.draw(sprite);
        
    //     char str[100];
    //     sprintf(str, "max_iter: %d\nzoom:x%2.2lf", max_iter, zoom);
    //     text.setString(str);
    //     window.draw(text);

    //     window.display();
    // }
}