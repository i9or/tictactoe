#pragma once

#include <array>
#include <vector>

enum class Mark {
  Empty,
  Nought,
  Cross
};

enum class GameState {
  CarryOn,
  Win,
  Draw
};

class Game {
public:
  Game();

  void update(sf::Vector2u);
  bool isPlayable();
  bool isDraw();
  bool haveCrossesWon();
  bool haveNoughtsWon();

  bool isCrossesTurn();
  bool isNoughtsTurn();

  bool isCrossHere(unsigned int, unsigned int);
  bool isNoughtHere(unsigned int, unsigned int);

  [[nodiscard]] std::vector<sf::Vector2u> getWinningLine() const;

private:
  void updateGameState();

private:
  std::array<std::array<Mark, 3>, 3> m_field;
  Mark m_currentTurn;
  GameState m_gameState;
  std::vector<sf::Vector2u> m_winningLine;
};
