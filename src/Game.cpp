//
//  Game.cpp
//  tictactoe
//
//  Created by Igor on 30/07/2023.
//

#include "Game.hpp"

Game::Game()
{
  for (std::size_t i = 0; i < FIELD_SIZE; ++i) {
    for (std::size_t j = 0; j < FIELD_SIZE; ++j) {
      field[i][j] = Marks::EMPTY;
    }
  }
}

Game::~Game() { }
