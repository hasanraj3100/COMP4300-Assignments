#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Window.hpp>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

class AShape {
public:
  std::shared_ptr<sf::Shape> shape;
  sf::Text name;
  float x, y, sx, sy;
  int h, w;

  AShape(const std::shared_ptr<sf::Shape> &shape, const sf::Text &name, float x,
         float y, float sx, float sy, int h, int w)
      : shape(shape), name(name), x(x), y(y), sx(sx), sy(sy), h(h), w(w) {
    centerText();
  }
  void move(float window_W, float window_H) {

    x += sx;
    y += sy;

    if (x < 0) {
      x = 0;
      sx = -sx;
    } else if (x + w > window_W) {
      x = window_W - w;
      sx = -sx;
    }

    if (y < 0) {
      y = 0;
      sy = -sy;
    } else if (y + h > window_H) {
      y = window_H - h;
      sy = -sy;
    }
    shape->setPosition(x, y);
    centerText();
  }

  void changeSX() {
    this->sx = -sx;
    std::cout << "sx is now " << sx << std::endl;
  }

  void changeSY() { this->sy = -sy; }

  void centerText() {
    float nx = name.getLocalBounds().width;
    float ny = name.getLocalBounds().height;
    float xpos = x + (float)w / 2 - nx / 2;
    float ypos = y + (float)h / 2 - (float)name.getCharacterSize() / 2;
    name.setPosition(sf::Vector2f(xpos, ypos));
  }

  const std::shared_ptr<sf::Shape> &getShape() { return this->shape; }
  const sf::Text &getText() { return this->name; }
};

std::vector<AShape> shapes;

void handleInput(std::fstream &fin, sf::Font &font, int fontSize, int fr,
                 int fg, int fb) {
  std::string temp;

  while (fin >> temp) {

    if (temp == "Circle") {
      std::string name;
      float x, y;
      float sx, sy;
      int r, g, b;
      int rad;
      fin >> name >> x >> y >> sx >> sy >> r >> g >> b >> rad;

      std::shared_ptr<sf::Shape> circle =
          std::make_shared<sf::CircleShape>(rad);
      circle->setFillColor(sf::Color(r, g, b));
      circle->setPosition(sf::Vector2f(x, y));

      sf::Text text;
      text.setString(name);

      AShape s(circle, text, x, y, sx, sy, circle->getLocalBounds().height,
               circle->getLocalBounds().width);

      shapes.push_back(s);

    }

    else if (temp == "Rectangle") {
      std::string name;
      float x, y, sx, sy;
      int r, g, b, h, w;

      fin >> name >> x >> y >> sx >> sy >> r >> g >> b >> w >> h;

      std::shared_ptr<sf::Shape> rec =
          std::make_shared<sf::RectangleShape>(sf::Vector2f(w, h));
      rec->setFillColor(sf::Color(r, g, b));
      rec->setPosition(sf::Vector2f(x, y));

      sf::Text text;
      text.setString(name);

      AShape s(rec, text, x, y, sx, sy, h, w);
      shapes.push_back(s);
    }

    else if (temp == "Font") {
      std::string location;

      fin >> location >> fontSize >> fr >> fg >> fb;

      if (!font.loadFromFile(location)) {
        std::cerr << "Font file not found" << std::endl;
      }
      font.loadFromFile(location);
    }
  }

  for (auto &shape : shapes) {

    shape.name.setFont(font);
    shape.name.setFillColor(sf::Color(fr, fg, fb));
    shape.name.setCharacterSize(fontSize);
  }
}

int main() {

  std::fstream fin("config.txt");
  std::string temp;
  float window_W, window_H;
  fin >> temp >> window_W >> window_H;

  sf::Font font;
  int fr, fg, fb, fontSize;

  handleInput(fin, font, fontSize, fr, fg, fb);

  sf::RenderWindow window(sf::VideoMode(window_W, window_H), "Mygame");

  while (window.isOpen()) {

    sf::Event event;

    while (window.pollEvent(event)) {

      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.clear();
    for (auto &shape : shapes) {
      window.draw(*(shape.getShape()));
      window.draw(shape.getText());
      shape.move(window_W, window_H);
    }

    window.display();
  }

  return 0;
}
