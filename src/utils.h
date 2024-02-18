#pragma once

#include <SFML/System/Vector2.hpp>

#include "utils.h"

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

