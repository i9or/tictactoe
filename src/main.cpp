#include <iostream>

#include <SFML/Graphics.hpp>

constexpr auto WINDOW_SIZE = 512u;
constexpr auto THIRD_OF_WINDOW = WINDOW_SIZE / 3.f;

int main() {
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  sf::RenderWindow window{
      {WINDOW_SIZE, WINDOW_SIZE}, "Tic Tac Toe", sf::Style::Default, settings};

  sf::Image gridImage;
  sf::Image circleImage;
  sf::Image crossImage;
  gridImage.loadFromFile("../assets/grid.png");
  circleImage.loadFromFile("../assets/o.png");
  crossImage.loadFromFile("../assets/x.png");

  circleImage.createMaskFromColor(sf::Color::White);
  crossImage.createMaskFromColor(sf::Color::White);

  sf::Texture gridTexture;
  gridTexture.loadFromImage(gridImage);
  sf::Sprite grid{gridTexture};

  sf::Texture circleTexture;
  circleTexture.loadFromImage(circleImage);
  sf::Sprite circle{circleTexture};
  circle.setScale(0.7f, 0.7f);

  sf::Texture crossTexture;
  crossTexture.loadFromImage(crossImage);
  sf::Sprite cross{crossTexture};
  cross.setScale(0.7f, 0.7f);
  cross.move(THIRD_OF_WINDOW, THIRD_OF_WINDOW);

  while (window.isOpen()) {
    sf::Event event{};
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }

      if (event.type == sf::Event::KeyPressed &&
          (event.key.code == sf::Keyboard::Escape)) {
        window.close();
      }
    }

    window.clear(sf::Color::Green);

    window.draw(grid);
    window.draw(circle);
    window.draw(cross);

    window.display();
  }

  return 0;
}
