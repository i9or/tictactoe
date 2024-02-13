#include <cmath>
#include <iostream>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

constexpr auto WINDOW_SIZE = 512u;
constexpr auto THIRD_OF_WINDOW = WINDOW_SIZE / 3.f;

enum class Mark { Empty, Nought, Cross };
enum class GameState { CarryOn, Win, Draw };

const std::vector<std::vector<sf::Vector2u>> WINING_CONDITIONS = {
    {{{0, 0}, {0, 1}, {0, 2}},
     {{1, 0}, {1, 1}, {1, 2}},
     {{2, 0}, {2, 1}, {2, 2}},
     {{0, 0}, {1, 0}, {2, 0}},
     {{0, 1}, {1, 1}, {2, 1}},
     {{0, 2}, {1, 2}, {2, 2}},
     {{0, 0}, {1, 1}, {2, 2}},
     {{0, 2}, {1, 1}, {2, 0}}}};

sf::Vector2u resolveFieldPosition(sf::Vector2i windowPosition) {
  return {windowPosition.x / static_cast<unsigned int>(THIRD_OF_WINDOW),
          windowPosition.y / static_cast<unsigned int>(THIRD_OF_WINDOW)};
}

std::tuple<GameState, std::vector<sf::Vector2u>>
checkWinFor(Mark mark, const std::array<std::array<Mark, 3>, 3> &field) {
  for (const auto &condition : WINING_CONDITIONS) {
    if (std::all_of(condition.begin(), condition.end(),
                    [field, mark](auto position) {
                      return field[position.x][position.y] == mark;
                    })) {
      return {GameState::Win, condition};
    }
  }

  for (const auto &row : field) {
    for (const auto &element : row) {
      if (element == Mark::Empty) {
        return {GameState::CarryOn, {}};
      }
    }
  }

  return {GameState::Draw, {}};
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

  std::array<std::array<Mark, 3>, 3> field{};
  Mark currentTurn = Mark::Cross;
  GameState gameState = GameState::CarryOn;
  std::vector<sf::Vector2u> winningLine = {};

  window.setTitle("Tic Tac Toe: crosses turn");

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
        if (event.mouseButton.button == sf::Mouse::Left &&
            gameState == GameState::CarryOn) {
          auto mousePosition = sf::Mouse::getPosition(window);
          auto [x, y] = resolveFieldPosition(mousePosition);

          if (field[x][y] == Mark::Empty) {
            field[x][y] = currentTurn;

            auto [outcome, line] = checkWinFor(currentTurn, field);
            gameState = outcome;
            winningLine = line;

            if (gameState == GameState::CarryOn) {
              if (currentTurn == Mark::Cross) {
                currentTurn = Mark::Nought;
                window.setTitle("Tic Tac Toe: noughts turn");
              } else {
                currentTurn = Mark::Cross;
                window.setTitle("Tic Tac Toe: crosses turn");
              }
            } else if (gameState == GameState::Draw) {
              window.setTitle("Tic Tac Toe: it's a draw!");
            } else if (gameState == GameState::Win) {
              if (currentTurn == Mark::Cross) {
                window.setTitle("Tic Tac Toe: crosses won!");
              } else {
                window.setTitle("Tic Tac Toe: noughts won!");
              }
            }
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
        if (field[row][column] == Mark::Cross) {
          cross.setPosition(markPosition);
          window.draw(cross);
        } else if (field[row][column] == Mark::Nought) {
          nought.setPosition(markPosition);
          window.draw(nought);
        }
      }
    }

    if (gameState == GameState::Draw) {
      window.draw(drawSprite);
    }

    if (gameState == GameState::Win) {
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

      window.draw(winStroke, &winTexture);
    }

    window.display();
  }

  return EXIT_SUCCESS;
}
