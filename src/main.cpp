#include <SFML/Graphics.hpp>

int main()
{
  sf::VideoMode videoMode { sf::Vector2u { 300, 300 } };
  sf::RenderWindow window { videoMode, "Tic Tac Toe!!" };

  while (window.isOpen()) {
    sf::Event event {};
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }

      if (event.type == sf::Event::KeyPressed
          && (event.key.code == sf::Keyboard::Escape)) {
        window.close();
      }
    }

    window.clear(sf::Color::Black);
    window.display();
  }

  return EXIT_SUCCESS;
}
