#pragma once
#include <random>
#include <utility>
#include <vector>

namespace ising {

struct ising_model {
  // enum type : int { dead = 0, alive = 1 };
  using type = int;

  ising_model() = default;
  ising_model(const ising_model&) = default;
  ising_model& operator=(const ising_model&) = default;
  ising_model(ising_model&&) = default;
  ising_model& operator=(ising_model&&) = default;
  ~ising_model() = default;

  ising_model(int r, int c, float b = 1.0)
      : rows{r}, cols{c}, beta{b}, cells(r * c, 0), buffer(r * c) {}

  constexpr auto map(int i, int j) const noexcept { return i * cols + j; }
  constexpr auto periodic_map(int i, int j) const noexcept {
    return ((i + rows) % rows) * cols + ((j + cols) % cols);
  }
  auto& operator()(int i, int j) noexcept { return cells[map(i, j)]; }
  const auto& operator()(int i, int j) const noexcept {
    return cells[map(i, j)];
  }
  auto& get(int i, int j) noexcept { return cells[periodic_map(i, j)]; }
  const auto& get(int i, int j) const noexcept {
    return cells[periodic_map(i, j)];
  }
  void clear() noexcept { std::fill(cells.begin(), cells.end(), 0); }
  int size() const noexcept { return rows * cols; }

  int rows{};
  int cols{};
  float beta{1.0};
  std::vector<type> cells{};
  decltype(cells) buffer{};
};

template <typename RNG>
inline void random_generate(ising_model& model, RNG& rng) noexcept {
  std::uniform_int_distribution<int> dist{0, 1};
  for (int i = 0; i < model.rows; ++i) {
    for (int j = 0; j < model.cols; ++j) {
      model(i, j) = dist(rng);
    }
  }
}

template <typename RNG>
inline void advance(ising_model& model, RNG& rng) noexcept {
  std::uniform_int_distribution<int> dist{0, model.rows * model.cols};
  std::uniform_real_distribution<float> e_dist{0, 1};
  for (int it = 0; it < model.rows * model.cols; ++it) {
    const auto index = dist(rng);
    const auto i = index / model.cols;
    const auto j = index % model.cols;
    const auto cost = 2 * (2 * model(i, j) - 1) *
                      (2 * (model.get(i + 1, j) + model.get(i - 1, j) +
                            model.get(i, j + 1) + model.get(i, j - 1)) -
                       4);
    if (cost < 0)
      model(i, j) = !model(i, j);
    else if (e_dist(rng) < std::exp(-cost * model.beta))
      model(i, j) = !model(i, j);

    // const int param = e_dist(rng) < std::exp(-cost * model.beta);
    // model(i, j) = (1 - param) * model(i, j) + param * !model(i, j);
  }
}

}  // namespace ising