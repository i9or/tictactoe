#include <cmath>
#include <string>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "Game.h"

constexpr auto WINDOW_SIZE = 512u;
constexpr auto THIRD_OF_WINDOW = WINDOW_SIZE / 3.f;
const std::string TITLE = "Tic Tac Toe";

sf::Vector2u resolveFieldPosition(sf::Vector2i windowPosition) {
  return {windowPosition.x / static_cast<unsigned int>(THIRD_OF_WINDOW),
          windowPosition.y / static_cast<unsigned int>(THIRD_OF_WINDOW)};
}

std::string resolveTitle(const std::string &value) {
  return TITLE + ": " + value;
}

void updateWinStrokePosition(sf::VertexArray &winStroke, const std::vector<sf::Vector2u> &winningLine) {
  sf::Vector2f v0{winningLine[0]};
  sf::Vector2f v1{winningLine[2]};

  sf::Vector2f p0{THIRD_OF_WINDOW * v0.x + THIRD_OF_WINDOW / 2.f,
                  THIRD_OF_WINDOW * v0.y + THIRD_OF_WINDOW / 2.f};
  sf::Vector2f p1{THIRD_OF_WINDOW * v1.x + THIRD_OF_WINDOW / 2.f,
                  THIRD_OF_WINDOW * v1.y + THIRD_OF_WINDOW / 2.f};

  // Finding direction of the wining stroke
  sf::Vector2f D = p1 - p0;

  // Normalize direction vector
  D /= sqrtf(powf(D.x, 2.f) + powf(D.y, 2.f));

  // Finding perpendicular to the direction
  sf::Vector2f N{-D.y, D.x};

  // Defining rectangle width
  auto w = 25.f;
  // Defining rectangle extension on each end
  auto e = 75.f;

  // Calculating rectangle corners
  auto c0 = p0 + w * N - e * D;
  auto c1 = p0 - w * N - e * D;
  auto c2 = p1 + w * N + e * D;
  auto c3 = p1 - w * N + e * D;

  winStroke[0].position = c0;
  winStroke[1].position = c2;
  winStroke[2].position = c1;
  winStroke[3].position = c2;
  winStroke[4].position = c1;
  winStroke[5].position = c3;
}
int main() {
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  sf::RenderWindow window{{WINDOW_SIZE, WINDOW_SIZE},
                          TITLE,
                          sf::Style::Titlebar | sf::Style::Close,
                          settings};

  sf::Image gridImage;
  sf::Image circleImage;
  sf::Image crossImage;
  sf::Image drawImage;
  sf::Image winImage;
  if (!gridImage.loadFromFile("../assets/grid.png") ||
      !circleImage.loadFromFile("../assets/o.png") ||
      !crossImage.loadFromFile("../assets/x.png") ||
      !drawImage.loadFromFile("../assets/draw.png") ||
      !winImage.loadFromFile("../assets/win.png")) {
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

  sf::Texture drawTexture;
  drawTexture.loadFromImage(drawImage);
  sf::Sprite drawSprite{drawTexture};

  sf::Texture winTexture;
  winTexture.loadFromImage(winImage);

  sf::Music music;
  if (!music.openFromFile(
          "../assets/Komiku_-_03_-_Mushrooms(chosic.com).mp3")) {
    return EXIT_FAILURE;
  }

  music.setLoop(true);
  music.play();

  sf::VertexArray winStroke{sf::Triangles, 6};

  sf::Vector2f winTextureSize{
      static_cast<float>(winTexture.getSize().x - 1),
      static_cast<float>(winTexture.getSize().y - 1),
  };

  winStroke[0].texCoords = sf::Vector2f(0.f, 0.f);
  winStroke[1].texCoords = sf::Vector2f(0.f, winTextureSize.y);
  winStroke[2].texCoords = sf::Vector2f(winTextureSize.x, 0.f);
  winStroke[3].texCoords = sf::Vector2f(0.f, winTextureSize.y);
  winStroke[4].texCoords = sf::Vector2f(winTextureSize.x, 0.f);
  winStroke[5].texCoords = sf::Vector2f(winTextureSize.x, winTextureSize.y);

  Game game;

  window.setTitle(resolveTitle("crosses turn"));

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
        if (event.mouseButton.button == sf::Mouse::Left && game.isPlayable()) {
          auto mousePosition = sf::Mouse::getPosition(window);
          auto fieldPositionToUpdate = resolveFieldPosition(mousePosition);

          game.update(fieldPositionToUpdate);

          if (game.isPlayable()) {
            if (game.isCrossesTurn()) {
              window.setTitle(resolveTitle("noughts turn"));
            } else {
              window.setTitle(resolveTitle("crosses turn"));
            }
          } else if (game.isDraw()) {
            window.setTitle(resolveTitle("it's a draw!"));
          } else if (game.haveCrossesWon()) {
            window.setTitle(resolveTitle("crosses won!"));
          } else if (game.haveNoughtsWon()) {
            window.setTitle(resolveTitle("noughts won!"));
          }
        }
        break;
      default:
        break;
      }
    }

    window.clear(sf::Color::White);

    window.draw(grid);

    // Draw field state
    for (size_t row = 0; row < 3; ++row) {
      for (size_t column = 0; column < 3; ++column) {
        sf::Vector2f markPosition{static_cast<float>(row) * THIRD_OF_WINDOW,
                                  static_cast<float>(column) * THIRD_OF_WINDOW};

        if (game.isCrossHere(row, column)) {
          cross.setPosition(markPosition);
          window.draw(cross);
        } else if (game.isNoughtHere(row, column)) {
          nought.setPosition(markPosition);
          window.draw(nought);
        }
      }
    }

    if (game.isDraw()) {
      window.draw(drawSprite);
    }

    if (game.haveCrossesWon() || game.haveNoughtsWon()) {
      auto winningLine = game.getWinningLine();
      updateWinStrokePosition(winStroke, winningLine);

      window.draw(winStroke, &winTexture);
    }

    window.display();
  }

  return EXIT_SUCCESS;
}
