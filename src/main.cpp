#include <iostream>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

constexpr auto WINDOW_SIZE = 512u;
constexpr auto THIRD_OF_WINDOW = WINDOW_SIZE / 3.f;

enum class SpaceState { Empty, Nought, Cross };

std::pair<int, int> resolveFieldPosition(sf::Vector2i windowPosition) {
  auto x = windowPosition.x / static_cast<int>(THIRD_OF_WINDOW);
  auto y = windowPosition.y / static_cast<int>(THIRD_OF_WINDOW);

  return std::make_pair(x, y);
}

void printField(const std::array<std::array<SpaceState, 3>, 3> &arr) {
  std::cout << "===" << std::endl;
  for (const auto &row : arr) {
    for (const auto &element : row) {
      if (element == SpaceState::Empty) {
        std::cout << ".";
      } else if (element == SpaceState::Nought) {
        std::cout << "o";
      } else {
        std::cout << "x";
      }
    }
    std::cout << std::endl;
  }
}

int main() {
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  sf::RenderWindow window{{WINDOW_SIZE, WINDOW_SIZE},
                          "Tic Tac Toe",
                          sf::Style::Titlebar | sf::Style::Close,
                          settings};

  sf::Image gridImage;
  sf::Image circleImage;
  sf::Image crossImage;
  if (!gridImage.loadFromFile("../assets/grid.png")) {
    return EXIT_FAILURE;
  }
  if (!circleImage.loadFromFile("../assets/o.png")) {
    return EXIT_FAILURE;
  }
  if (!crossImage.loadFromFile("../assets/x.png")) {
    return EXIT_FAILURE;
  }

  sf::Texture gridTexture;
  gridTexture.loadFromImage(gridImage);
  sf::Sprite grid{gridTexture};

  sf::Texture noughtTexture;
  noughtTexture.loadFromImage(circleImage);
  sf::Sprite nought{noughtTexture};
  nought.setScale(0.33f, 0.33f);

  sf::Texture crossTexture;
  crossTexture.loadFromImage(crossImage);
  sf::Sprite cross{crossTexture};
  cross.setScale(0.33f, 0.33f);

  sf::Music music;
  if (!music.openFromFile(
          "../assets/Komiku_-_03_-_Mushrooms(chosic.com).mp3")) {
    return EXIT_FAILURE;
  }

  music.play();

  std::array<std::array<SpaceState, 3>, 3> field{};
  SpaceState currentTurn = SpaceState::Cross;

  while (window.isOpen()) {
    sf::Event event{};
    while (window.pollEvent(event)) {
      switch (event.type) {
      case sf::Event::Closed:
        window.close();
        break;
      case sf::Event::KeyPressed:
        if (event.key.code == sf::Keyboard::Escape) {
          window.close();
        }
        break;
      case sf::Event::MouseButtonReleased:
        if (event.mouseButton.button == sf::Mouse::Left) {
          auto mousePosition = sf::Mouse::getPosition(window);
          auto [x, y] = resolveFieldPosition(mousePosition);

          field[x][y] = currentTurn;

          currentTurn = currentTurn == SpaceState::Cross ? SpaceState::Nought
                                                         : SpaceState::Cross;
        }
        break;
      default:
        break;
      }
    }

    window.clear(sf::Color::White);

    window.draw(grid);

    //    window.draw(nought);
    //    window.draw(cross);
    for (size_t row = 0; row < 3; ++row) {
      for (size_t column = 0; column < 3; ++column) {
        sf::Vector2f shapePosition{row * THIRD_OF_WINDOW,
                                   column * THIRD_OF_WINDOW};
        if (field[row][column] == SpaceState::Cross) {
          cross.setPosition(shapePosition);
          window.draw(cross);
        } else if (field[row][column] == SpaceState::Nought) {
          nought.setPosition(shapePosition);
          window.draw(nought);
        }
      }
    }

    window.display();
  }

  return EXIT_SUCCESS;
}
