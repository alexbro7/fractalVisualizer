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


std::map<const std::string, std::vector<sf::Color> > loadColors(){
    std::ifstream ifs; ifs.open("colorPalett.txt");
    std::map<const std::string, std::vector<sf::Color> > colors;
    std::string            fileContent, s, nTmp;
    std::vector<sf::Color> vTmp;
    int                    rTmp = 0, gTmp = 0, bTmp = 0;
    if (!ifs)
    {
        std::cerr << "Can't open colorPalett.txt" << std::endl;
        vTmp.push_back(sf::Color::Black);
        vTmp.push_back(sf::Color::White);
        colors.insert(std::make_pair("black and white", vTmp));
        return colors;
    }
    getline(ifs, s);
    fileContent += s;
    while (getline(ifs, s)){
      fileContent += " ";
      fileContent += s;
    }
    ifs.close();
    if (fileContent == ""){
        std::cerr << "colorPalett.txt is empty" << std::endl;
        vTmp.push_back(sf::Color::Black);
        vTmp.push_back(sf::Color::White);
        colors.insert(std::make_pair("black and white", vTmp));
        return colors;
    }
    int l = 0;
    bool b = 0;
    std::string::size_type i = 0, j;
    while (std::isblank(fileContent[i]))if (fileContent[i] == '\n') l++; i++;
    for (; i < fileContent.length(); i++){
        if (std::isdigit(fileContent[i])) b = 1; break;
        j = i;
        while (!std::isblank(fileContent[i])) i++;
        nTmp = fileContent.substr(j, i - j);
        if (nTmp.empty()) b = 1; break;
        while (std::isblank(fileContent[i]))if (fileContent[i] == '\n') l++; i++;
        if (!std::isdigit(fileContent[i])) b = 1; break;
        while (std::isdigit(fileContent[i])) {
            if (rTmp > 99) b = 1; break;
            rTmp *= 10;
            rTmp += fileContent[i] - '0';
            if (rTmp > 255) b = 1; break;
            i++;
            while (std::isblank(fileContent[i]))if (fileContent[i] == '\n') l++; i++;
            if (i == fileContent.length() || fileContent[i] != ',') b = 1; break;
            i++;
            while (std::isblank(fileContent[i]))if (fileContent[i] == '\n') l++; i++;
            if (!std::isdigit(fileContent[i])) b = 1; break;
            while (std::isdigit(fileContent[i])) {
                if (gTmp > 99) b = 1; break;
                gTmp *= 10;
                gTmp += fileContent[i] - '0';
                if (gTmp > 255) b = 1; break;
                i++;
            }
            while (std::isblank(fileContent[i]))if (fileContent[i] == '\n') l++; i++;
            if (i == fileContent.length() || fileContent[i] != ',') b = 1; break;
            i++;
            while (std::isblank(fileContent[i]))if (fileContent[i] == '\n') l++; i++;
            if (!std::isdigit(fileContent[i])) b = 1; break;
            while (std::isdigit(fileContent[i])) {
                if (bTmp > 99) b = 1; break;
                bTmp *= 10;
                bTmp += fileContent[i] - '0';
                if (bTmp > 255) b = 1; break;
                i++;
            }
            while (std::isblank(fileContent[i]))if (fileContent[i] == '\n') l++; i++;
            vTmp.push_back(sf::Color(rTmp,gTmp,bTmp)); rTmp = gTmp = bTmp = 0;
        }
        colors.insert(std::make_pair(nTmp, vTmp)); nTmp.clear(); vTmp.clear();
    }
    if (b){
        std::cerr << "Error: colorPalett.txt line " << l << std::endl;
        vTmp.push_back(sf::Color::Black);
        vTmp.push_back(sf::Color::White);
        colors.insert(std::make_pair("black and white", vTmp));
        return colors;
    }
    if (vTmp.empty()) {
        std::cerr << "colorPalett.txt is empty" << std::endl;
        vTmp.push_back(sf::Color::Black);
        vTmp.push_back(sf::Color::White);
        colors.insert(std::make_pair("black and white", vTmp));
        return colors;
    }
    return colors;
}

int main(void){
    std::map<const std::string, std::vector<sf::Color> >colors = loadColors();
    for (std::map<const std::string, std::vector<sf::Color> >::const_iterator it = colors.begin(); it != colors.end(); it++){
        std::cout << it->first << std::endl;
        for (std::vector<sf::Color>::const_iterator jt = it->second.begin(); jt != it->second.end(); jt++)
            std::cout << "[" << jt->r << ", " << jt->g << ", " << jt->b << "]" << std::endl;
        std::cout << std::endl;
    }
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