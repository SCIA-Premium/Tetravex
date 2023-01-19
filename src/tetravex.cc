#include "tetravex.hh"

void Tetravex::readFromFile(std::string filename)
{
    std::ifstream inputFile(filename);
    if (!inputFile)
    {
        std::cout << "Error opening input file" << std::endl;
        exit(1);
    }
    
    std::string line;
    while (std::getline(inputFile, line))
    {
        this->size += 1;
    }
    this->size = sqrt(this->size);
    inputFile.close();

    inputFile.open(filename);
    this->tiles = std::vector<Tile>(this->size * this->size);
    int tileCount = 0;
    while (std::getline(inputFile, line))
    {
        if (line.empty())
            continue;
        int numbers[4];
        for (int i = 0; i < line.size(); ++i)
        {
            if (line[i] >= '0' && line[i] <= '9')
                numbers[i] = line[i] - '0';
        }
        this->tiles[tileCount].north = numbers[0];
        this->tiles[tileCount].west = numbers[1];
        this->tiles[tileCount].east = numbers[2];
        this->tiles[tileCount].south = numbers[3];

        if (line[line.size() - 1] == '@')
            this->tiles[tileCount].inPlace = true;
        else
            this->tiles[tileCount].inPlace = false;

        tileCount++;
    }
}

void Tetravex::writeToFile(std::string filename) const
{
    std::ofstream outputFile(filename);
    if (!outputFile)
    {
        std::cout << "Error opening output file" << std::endl;
        exit(1);
    }

    for (int i = 0; i < this->getSize() * this->getSize(); i++)
    {
        outputFile << this->tiles[i].north << this->tiles[i].west << this->tiles[i].east
                   << this->tiles[i].south << std::endl;
    }

    outputFile.close();
}

std::ostream &operator<<(std::ostream &os, const Tetravex &tetravex)
{
    int size = tetravex.getSize();
    for (int i = 0; i < size; i++) {
        os << "+" << "-----";
    }
    os << "+" << std::endl;
    for (int i = 0; i < size; i++) {
        for (int tile_orientation = 0; tile_orientation < 3; tile_orientation++) {
            std::cout << "|";
            for (int j = 0; j < size; j++) {
                os << " ";
                // Print in yellow if the tile is in place
                if (tetravex[i * size + j].inPlace) {
                    os << "\033[1;33m";
                }
                if (tile_orientation == 0) {
                    os << " " << tetravex[i * size + j].north << "  ";
                }
                else if (tile_orientation == 1) {
                    os << tetravex[i * size + j].west << " ";
                    os << tetravex[i * size + j].east << " ";
                }
                else if (tile_orientation == 2) {
                    os << " " << tetravex[i * size + j].south << "  ";
                }
                // Reset color
                if (tetravex[i * size + j].inPlace) {
                    os << "\033[0m";
                }
                os << "|";
            }
            os << std::endl;
        }
        if (i != size - 1) {
            for (int i = 0; i < size; i++) {
                os << "+" << "-----";
            }
            os << "+" << std::endl;
        }
    }
    for (int i = 0; i < size; i++) {
        os << "+" << "-----";
    }
    os << "+" << std::endl;
    return os;
}