//
//  Game.hpp
//  tictactoe
//
//  Created by Igor on 30/07/2023.
//

#ifndef Game_hpp
#define Game_hpp

#include <cstddef>

enum class Marks { NOUGHT, CROSS, EMPTY };

const std::size_t FIELD_SIZE = 3;

class Game {
private:
  Marks field[FIELD_SIZE][FIELD_SIZE];

public:
  Game();
  ~Game();
};

#endif /* Game_hpp */
