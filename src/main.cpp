#include <cmath>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "Game.h"
#include "constants.h"
#include "utils.h"

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
