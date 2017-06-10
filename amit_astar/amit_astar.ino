 #include "astar.h"

void setup() {
  SquareGrid grid = make_diagram1();
  auto parents = breadth_first_search(grid, std::make_tuple(7, 8));
}

}

void loop() {
  // put your main code here, to run repeatedly:

}
