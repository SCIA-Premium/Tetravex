#include "solver.hh"
#include "tetravex.hh"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " input_file output_file"
                  << std::endl;
        exit(1);
    }

    Tetravex tetravex = Tetravex();
    tetravex.readFromFile(argv[1]);   
#ifndef NDEBUG
    std::cout << tetravex << std::endl;
#endif
    solveTetravex(tetravex);
    tetravex.writeToFile(argv[2]);
#ifndef NDEBUG
    std::cout << std::endl << tetravex << std::endl;
#endif
    return 0;
}