#include <iostream>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

constexpr auto WINDOW_SIZE = 512u;
constexpr auto THIRD_OF_WINDOW = WINDOW_SIZE / 3.f;

enum class Mark { Empty, Nought, Cross };
enum class Outcome { CarryOn, Win, Draw };

const std::array<std::vector<sf::Vector2u>, 8> WINING_CONDITIONS = {
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

std::tuple<Outcome, std::vector<sf::Vector2u>>
checkWinFor(Mark mark, const std::array<std::array<Mark, 3>, 3> &field) {

  for (const auto &condition : WINING_CONDITIONS) {
    if (std::all_of(condition.begin(), condition.end(),
                    [field, mark](auto position) {
                      return field[position.x][position.y] == mark;
                    })) {
      return {Outcome::Win, condition};
    }
  }

  for (const auto &row : field) {
    for (const auto &element : row) {
      if (element == Mark::Empty) {
        return {Outcome::CarryOn, {}};
      }
    }
  }

  return {Outcome::Draw, {}};
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
  sf::Sprite draw{drawTexture};

  sf::Texture winTexture;
  winTexture.loadFromImage(winImage);

  sf::Music music;
  if (!music.openFromFile(
          "../assets/Komiku_-_03_-_Mushrooms(chosic.com).mp3")) {
    return EXIT_FAILURE;
  }

  music.setLoop(true);
  music.play();

  sf::VertexArray winStrip{sf::TriangleStrip, 4};

  winStrip[0].position = sf::Vector2f(0.f, 0.f);
  winStrip[1].position = sf::Vector2f(0.f, WINDOW_SIZE);
  winStrip[2].position = sf::Vector2f(THIRD_OF_WINDOW, 0.f);
  winStrip[3].position = sf::Vector2f(THIRD_OF_WINDOW, WINDOW_SIZE);
//  winStrip[0].color = sf::Color::Blue;
//  winStrip[1].color = sf::Color::Blue;
//  winStrip[2].color = sf::Color::Blue;
//  winStrip[3].color = sf::Color::Blue;

  winStrip[0].texCoords = sf::Vector2f(0.f, 0.f);
  winStrip[1].texCoords = sf::Vector2f(0.f, winTexture.getSize().y - 1);
  winStrip[2].texCoords = sf::Vector2f(winTexture.getSize().x - 1, 0.f);
  winStrip[3].texCoords = sf::Vector2f(winTexture.getSize().x - 1, winTexture.getSize().y - 1);

  std::array<std::array<Mark, 3>, 3> field{};
  Mark currentTurn = Mark::Cross;
  Outcome gameState = Outcome::CarryOn;
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
            gameState == Outcome::CarryOn) {
          auto mousePosition = sf::Mouse::getPosition(window);
          auto [x, y] = resolveFieldPosition(mousePosition);

          if (field[x][y] == Mark::Empty) {
            field[x][y] = currentTurn;

            auto [outcome, line] = checkWinFor(currentTurn, field);
            gameState = outcome;
            winningLine = line;

            if (gameState == Outcome::CarryOn) {
              if (currentTurn == Mark::Cross) {
                currentTurn = Mark::Nought;
                window.setTitle("Tic Tac Toe: noughts turn");
              } else {
                currentTurn = Mark::Cross;
                window.setTitle("Tic Tac Toe: crosses turn");
              }
            } else if (gameState == Outcome::Draw) {
              window.setTitle("Tic Tac Toe: it's a draw!");
            } else {
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
        sf::Vector2f shapePosition{row * THIRD_OF_WINDOW,
                                   column * THIRD_OF_WINDOW};
        if (field[row][column] == Mark::Cross) {
          cross.setPosition(shapePosition);
          window.draw(cross);
        } else if (field[row][column] == Mark::Nought) {
          nought.setPosition(shapePosition);
          window.draw(nought);
        }
      }
    }

    if (gameState == Outcome::Draw) {
      window.draw(draw);
    }

    window.draw(winStrip);

    window.display();
  }

  return EXIT_SUCCESS;
}
