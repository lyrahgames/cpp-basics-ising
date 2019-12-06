#pragma once
#include <SFML/Graphics.hpp>
#include <ising/ising_model.hpp>
#include <random>
#include <string>
#include <vector>

namespace ising {

class application {
 public:
  application(int rows = 500, int cols = 500, float temperature = 1.0);
  ~application() = default;

  void execute();

 private:
  void resize(int width, int height);
  void render();

 private:
  constexpr static size_t border = 5;
  sf::RenderWindow window;
  sf::View view;
  std::vector<uint8_t> pixels;
  sf::Texture texture{};
  sf::Sprite sprite{};

  ising_model model{500, 500};
  std::mt19937 rng{std::random_device{}()};
  // std::random_device rng{};
  // std::minstd_rand rng{std::random_device{}()};
  // std::ranlux48 rng{std::random_device{}()};
};

}  // namespace ising