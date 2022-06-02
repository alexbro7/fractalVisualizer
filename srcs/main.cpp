// #include <stdio.h>
// #include <iostream>
// #include <fstream>
// #include <vector>
// #include <map>
// #include <SFML/Graphics.hpp>

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

bool isBlank(const char& c) {return (c == ' ' || c == '\n' || c == '\t' || c == '\v' || c == '\f' || c == '\r') ? true : false;}

bool isDigit(const char& c) {return (c >= '0' && c <= '9') ? true : false;}

std::map<const std::string, std::vector<sf::Color> > loadColors() {
    std::map<const std::string, std::vector<sf::Color> >  colors;
    std::ifstream                       ifs("colorPalett", std::ios::in);
    if (!ifs) {std::cerr << "Error: can't open colorPalett." << std::endl; return colors;}
    std::string                         fileContent = "", s = "", nTmp = "";
    std::vector<sf::Color>              vTmp;
    sf::Color                           cTmp;
    int                                 rTmp, gTmp, bTmp, iTmp;
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
    if (fileContent.empty()) {std::cerr << "Error: colorPalett is empty" << std::endl; return colors;} else fileContent += ".;";
    for (std::string::size_type i = 0; i < fileContent.length(); i++){
        if (i) i -= 1;
        if (isDigit(fileContent[i]) == true && fileContent[i] != ';') {std::cerr << "Error: colorPalett:0 c:" << i << "." << fileContent[i] << std::endl; colors.clear(); return colors;}
        if (fileContent[i] == ';') return colors;
        for (;!isDigit(fileContent[i]); i++) {nTmp += fileContent[i]; if (i == fileContent.length()){std::cerr << "Error: colorPalett:1 c:" << i << "." << std::endl; colors.clear(); return colors;}}
        if (nTmp.empty()) {std::cerr << "Error: colorPalett:2 c:" << i << "." << std::endl; colors.clear(); return colors;}
std::cout << nTmp << std::endl;
        for (int rTmp = 0, gTmp = 0, bTmp = 0, iTmp;i < fileContent.length() && isDigit(fileContent[i]); i++) {
            for (char a = 0; a < 3; a++) {
                iTmp = 0;
                for (;i < fileContent.length() && isDigit(fileContent[i]); i++){
                    iTmp = iTmp * 10 + (fileContent[i] - '0'); if (iTmp > 255) {std::cerr << "Error: colorPalett:4 c:" << i << "." << std::endl; colors.clear(); return colors;}}
                if (i == s.length() || (a < 2 && fileContent[i] != ',') || (a == 2 && fileContent[i] != '.')) {std::cerr << "Error: colorPalett:5 c:" << i << "." << std::endl; colors.clear(); return colors;} else i++;
                if (a == 0) {rTmp = iTmp;} else if (a == 1) {gTmp = iTmp;} else {bTmp = iTmp;}
                if (a == 2) i -= 1;}
std::cout << rTmp << "," << gTmp << "," << bTmp << std::endl;
            vTmp.push_back(sf::Color(static_cast<sf::Uint8>(rTmp), static_cast<sf::Uint8>(gTmp), static_cast<sf::Uint8>(bTmp)));}
        colors.insert(std::make_pair(std::string(nTmp), std::vector<sf::Color>(vTmp))); nTmp.clear(); vTmp.clear();}
    return colors;
}

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

int main(void){
    sf::RenderWindow    window(sf::VideoMode(W, H), "Mandelbrot");
    sf::Image image; image.create(W,H);
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Font font; font.loadFromFile("Helvetica.ttc");
    sf::Text text; text.setFont(font); text.setCharacterSize(24); text.setFillColor(sf::Color::Red);
    std::map<const std::string, std::vector<sf::Color> > colors = loadColors();
    if (colors.empty()) return 1;
    std::map<const std::string, std::vector<sf::Color> >::const_iterator it = colors.begin();
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
                if (e.key.code == sf::Keyboard::Z) {if (colors.size() > 1) {if (it == colors.begin()) it = colors.end(); else it--;}}
                if (e.key.code == sf::Keyboard::X) {if (colors.size() > 1) {if (it == colors.end()) it = colors.begin(); else it++;}}
            }
            // if (e.type == sf::Event::MouseWheelScrolled){
                // if (e.MouseWheelScrolled){
                    // if (e.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel){
                        // if (e.mouseWheelScroll.delta > 0) max_iter *= 2;
                        // else max_iter /= 2;
                        // if (max_iter < 1) max_iter = 1;
                    // }
                // }
            // }
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
        drawMandelbrot(&image, it->second);
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

// int main(void){
//     std::map<const std::string, std::vector<sf::Color> >colors = loadColors();
//     sf::RenderWindow    window(sf::VideoMode(W, H), "Fractol");
//     sf::Image image; image.create(500,500);
//     sf::Texture texture;
//     sf::Sprite sprite;
//     sf::Font font; font.loadFromFile("Helvetica.ttc");
//     sf::Text text; text.setFont(font); text.setCharacterSize(24); text.setFillColor(sf::Color::White);
//     std::vector<std::vector<sf::Color> >colors = loadColors();
//     while (window.isOpen()){
//         sf::Event e;
//         while (window.pollEvent(e)){
//             if (e.type == sf::Event::Closed) window.close();
//             if (e.type == sf::Event::KeyPressed){
//                 if (e.key.code == sf::Keyboard::Escape) window.close();
//                 }
//             }
//             if (e.type == sf::Event::MouseButtonPressed){
//                     // double cr = min_re + (max_re - min_re) * e.mouseButton.x / W;
//                     // double ci = min_im + (max_im - min_im) * e.mouseButton.y / H;
//                 if (e.mouseButton.button == sf::Mouse::Left) {

//                 }
//                 // if (e.mouseButton.button == sf::Mouse::Right) {}
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
//     return 0;
// }