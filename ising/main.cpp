#include <iostream>
#include <ising/application.hpp>
#include <sstream>
#include <string>

using namespace std;

int main(int argc, char** argv) {
  int rows = 100;
  int cols = 100;
  float temperature = 1.0;

  if (argc >= 3) {
    stringstream input{argv[1]};
    input >> rows;
    input = stringstream{argv[2]};
    input >> cols;
    if (argc == 4) {
      input = stringstream{argv[3]};
      input >> temperature;
    }
  }

  cout << "Ising model resolution set to " << rows << " x " << cols
       << " with T = " << temperature << " !\n ";
  cout << "usage: " << argv[0] << " [<rows> <cols> [<temperature>]]\n";

  ising::application app{rows, cols, temperature};
  app.execute();
}