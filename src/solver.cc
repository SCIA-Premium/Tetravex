#include "solver.hh"

int calculateCost(std::vector<Tile> tiles, int size)
{
    int cost = 0;

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (i != 0 && tiles[i * size + j].north != tiles[(i - 1) * size + j].south)
                cost++;
            if (j != 0 && tiles[i * size + j].west != tiles[i * size + j - 1].east)
                cost++;
            if (i != size - 1 && tiles[i * size + j].south != tiles[(i + 1) * size + j].north)
                cost++;
            if (j != size - 1 && tiles[i * size + j].east != tiles[i * size + j + 1].west)
                cost++;
        }
    }
    return cost;
}

void transition(std::vector<Tile> &tiles, int size)
{
    int i = rand() % (size * size);
    while (tiles[i].inPlace)
        i = rand() % (size * size);
    
    int j = rand() % (size * size);
    while (tiles[j].inPlace && i == j)
        j = rand() % (size * size);

    std::swap(tiles[i], tiles[j]);
}

double init_T(Tetravex& t)
{
	double T1 = 0;
	double T2 = 1000000;
	double T = T2;

	std::vector<Tile> tiles= t.getTiles();
	int size = t.getSize();

	double eps_T = 1;

	while ((T2 - T1) > eps_T)
	{
		T = T1 + (T2 - T1) / 2;

		int uniform = 0;
		int nb_samples = 100;
	
		for (int i = 0; i < nb_samples; ++i)
		{
			double eps = 0.01;
			
            int x = rand() % (size * size);
            while (tiles[x].inPlace)
                x = rand() % (size * size);

            int x2 = rand() % (size * size);
            while (tiles[x2].inPlace && x == x2)
                x2 = rand() % (size * size);

            double c1 = calculateCost(t.getTiles(), t.getSize());
			std::swap(tiles[x], tiles[x2]);
			double c2 = calculateCost(t.getTiles(), t.getSize());
			
			if (exp(- (c2 - c1) / T) > 1 - eps)
				uniform += 1;
		}

		if (uniform >= 0.98 * nb_samples)
			T2 = T;
		else
			T1 = T;
	}
	return T;
}

void solveTetravex(Tetravex &tetravex)
{
    // paramètres de l'algorithme de recuit
    //double T = 250; // température initiale
    double T = init_T(tetravex);
    double Tmin = 0.8; // température minimale
    double alpha = 0.999; // taux de refroidissement
    
    int size = tetravex.getSize();
    int cost = calculateCost(tetravex.getTiles(), size);

    std::random_device rd;
    auto generator = std::mt19937(rd());
    auto distribution = std::uniform_real_distribution<double>(0.0f, 1.0f);
    while (cost)
    {
        std::vector<Tile> proposed = std::vector<Tile>(tetravex.getTiles());
        transition(proposed, size);
        int newCost = calculateCost(proposed, size);
        int delta = newCost - cost;
        if (delta <= 0)
        {
            cost = newCost;
            tetravex.setTiles(proposed);
        }
        else {
            double p = exp(-delta / T);
            double r = distribution(generator);
            if (r < p)
            {
                cost = newCost;
                tetravex.setTiles(proposed);
            }
        }
        if (cost == 0)
            return;
        
        if (T > Tmin)
			T *= alpha;
		else
			T = Tmin;
    }
}