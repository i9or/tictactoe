#include <SFML/System/Vector2.hpp>

#include "Game.h"

const std::vector<std::vector<sf::Vector2u>> WINING_CONDITIONS = {{
    {{0, 0}, {0, 1}, {0, 2}}, //
    {{1, 0}, {1, 1}, {1, 2}}, //
    {{2, 0}, {2, 1}, {2, 2}}, //
    {{0, 0}, {1, 0}, {2, 0}}, //
    {{0, 1}, {1, 1}, {2, 1}}, //
    {{0, 2}, {1, 2}, {2, 2}}, //
    {{0, 0}, {1, 1}, {2, 2}}, //
    {{0, 2}, {1, 1}, {2, 0}}  //
}};

Game::Game() : m_currentTurn(Mark::Cross),
               m_field({}),
               m_gameState(GameState::CarryOn),
               m_winningLine({}) {}

bool Game::isPlayable() {
  return m_gameState == GameState::CarryOn;
}

void Game::update(sf::Vector2u fieldPosition) {
  auto [x, y] = fieldPosition;

  if (m_field[x][y] == Mark::Empty) {
    m_field[x][y] = m_currentTurn;

    updateGameState();

    if (m_gameState == GameState::CarryOn) {
      if (m_currentTurn == Mark::Cross) {
        m_currentTurn = Mark::Nought;
      } else {
        m_currentTurn = Mark::Cross;
      }
    }
  }
}

void Game::updateGameState() {
  for (const auto &winningCondition : WINING_CONDITIONS) {
    auto predicate = [this](auto position) {
      return m_field[position.x][position.y] == m_currentTurn;
    };

    auto weHaveAWinner = std::all_of(winningCondition.begin(), winningCondition.end(), predicate);

    if (weHaveAWinner) {
      m_gameState = GameState::Win;
      m_winningLine = winningCondition;
      return;
    }
  }

  for (const auto &row : m_field) {
    for (const auto &element : row) {
      if (element == Mark::Empty) {
        m_gameState = GameState::CarryOn;
        m_winningLine = {};
        return;
      }
    }
  }

  m_gameState = GameState::Draw;
  m_winningLine = {};
}

bool Game::isDraw() {
  return m_gameState == GameState::Draw;
}

bool Game::haveCrossesWon() {
  return m_gameState == GameState::Win && isCrossesTurn();
}

bool Game::haveNoughtsWon() {
  return m_gameState == GameState::Win && isNoughtsTurn();
}

bool Game::isCrossesTurn() {
  return m_currentTurn == Mark::Cross;
}

bool Game::isNoughtsTurn() {
  return m_currentTurn == Mark::Nought;
}
std::vector<sf::Vector2u> Game::getWinningLine() const {
  return m_winningLine;
}

bool Game::isCrossHere(unsigned int row, unsigned int column) {
  return m_field[row][column] == Mark::Cross;
}

bool Game::isNoughtHere(unsigned int row, unsigned int column) {
  return m_field[row][column] == Mark::Nought;
}
