#include <cmath>
#include <iostream>
#include <ising/application.hpp>
#include <random>
#include <stdexcept>

namespace ising {

application::application(int rows, int cols, float temperature)
    : window{sf::VideoMode(cols + 2 * border, rows + 2 * border),
             "Ising Model"},
      pixels(rows * cols * 4),
      model{rows, cols, temperature} {
  random_generate(model, rng);

  texture.create(model.cols, model.rows);
  sprite.setTexture(texture, true);
  sprite.setOrigin(model.cols / 2.0, model.rows / 2.0);
}

void application::resize(int width, int height) {
  view.setCenter(0, 0);
  view.setSize(width, height);
  if (static_cast<float>(model.cols) / model.rows <
      static_cast<float>(width) / height) {
    view.zoom(static_cast<float>(model.rows) / (height - 2 * border));
  } else {
    view.zoom(static_cast<float>(model.cols) / (width - 2 * border));
  }
  window.setView(view);
}

void application::render() {
  for (int j = 0; j < model.rows; ++j) {
    for (int i = 0; i < model.cols; ++i) {
      const int index = 4 * (j * model.cols + i);
      pixels[index + 0] = pixels[index + 1] = pixels[index + 2] =
          255 * model(j, i);
      pixels[index + 3] = 255;
    }
  }
}

void application::execute() {
  bool update = false;
  bool play = false;

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          window.close();
          break;

        case sf::Event::Resized:
          resize(window.getSize().x, window.getSize().y);
          break;

        case sf::Event::KeyPressed:
          switch (event.key.code) {
            case sf::Keyboard::Escape:
              window.close();
              break;

            case sf::Keyboard::Space:
              play = !play;
              break;

            case sf::Keyboard::C:
              model.clear();
              break;

            case sf::Keyboard::I:
              update = true;
              break;

            case sf::Keyboard::R:
              random_generate(model, rng);
              break;
          }
      }
    }

    if (play || update) {
      ising::advance(model, rng);
      update = false;
    }

    window.clear(sf::Color::White);
    render();
    texture.update(pixels.data());
    window.draw(sprite);
    window.display();
  }
}

}  // namespace ising