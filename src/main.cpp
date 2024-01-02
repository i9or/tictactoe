#include <vector>

#include <SFML/Graphics.hpp>

constexpr auto WINDOW_SIZE = 300u;

int main() {
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  sf::RenderWindow window{
      {WINDOW_SIZE, WINDOW_SIZE}, "Tic Tac Toe", sf::Style::Default, settings};

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

    window.clear(sf::Color::Black);
    // TODO: Add drawing code here
    window.display();
  }

  return 0;
}
