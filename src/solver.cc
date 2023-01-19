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

// fonction pour effectuer une transition aléatoire entre les configurations
void transition(std::vector<Tile> &tiles, int size)
{


    int i = rand() % (size * size);
    while (tiles[i].inPlace)
        i = rand() % (size * size);
    
    int j = rand() % (size * size);
    while (tiles[j].inPlace && i == j)
        j = rand() % (size * size);

    std::swap(tiles[i], tiles[j]);
    // Compute the score of moving each tile
    /*std::vector<int> scores;
    for (int i = 0; i < size * size; i++) {
        if (tiles[i].inPlace) {
            scores.push_back(0);
            continue;
        }
        int score = 0;
        int x = i / size;
        int y = i % size;
        if (x > 0) {
            if (tiles[i].north != tiles[(x - 1) * size + y].south) {
                score++;
            }
        }
        if (y > 0) {
            if (tiles[i].west != tiles[x * size + y - 1].east) {
                score++;
            }
        }
        scores.push_back(score);
    }

    // Find the best tile to move
    int best_score = 0;
    int best_tile = 0;
    for (int i = 0; i < size * size; i++) {
        if (scores[i] > best_score) {
            best_score = scores[i];
            best_tile = i;
        }
    }

    // Find the best place to move it
    int best_place = 0;
    int best_place_score = 0;
    for (int i = 0; i < size * size; i++) {
        if (tiles[i].inPlace) {
            continue;
        }
        int score = 0;
        int x = i / size;
        int y = i % size;
        if (x > 0) {
            if (tiles[best_tile].north != tiles[(x - 1) * size + y].south) {
                score++;
            }
        }
        if (y > 0) {
            if (tiles[best_tile].west != tiles[x * size + y - 1].east) {
                score++;
            }
        }
        if (score > best_place_score && i != best_tile) {
            best_place_score = score;
            best_place = i;
        }
    }

    std::cout << "Moving tile " << best_tile << " to " << best_place << std::endl;
    std::swap(tiles[best_tile], tiles[best_place]);*/
}

std::pair<double, double> init_T(Tetravex& t)
{
	double T1 = 0;
	double T2 = 1000000;
	double T = T2;

	std::vector<Tile> pieces = t.getTiles();
	int n = t.getSize() * t.getSize();

	double eps_T = 1;

	while ((T2 - T1) > eps_T)
	{
		T = T1 + (T2 - T1) / 2;

		int uniform = 0;
		int nb_samples = 100;
	
		// check if uniform distribution
		// sample m times and check if transition proba is near 1
		for (int i = 0; i < nb_samples; ++i)
		{
			double eps_prob = 0.01;
			
			int i1 = rand() % n;
			int i2 = i1;
			while (i2 == i1)
				i2 = rand() % n;

			double U1 = calculateCost(t.getTiles(), t.getSize());
			std::iter_swap(pieces.begin() + i1, pieces.begin() + i2);
			double U2 = calculateCost(t.getTiles(), t.getSize());
			
			if (exp(- (U2 - U1) / T) > 1 - eps_prob)
				uniform += 1;
		}

		if (uniform >= 0.98 * nb_samples)
			T2 = T;
		else
			T1 = T;
	}

	return std::pair<double, double>(T, T1);
}

// fonction pour résoudre le tetravex en utilisant l'algorithme de recuit de
// Metropolis-Hastings
int solveTetravex(Tetravex &tetravex)
{
    srand(time(NULL));
    // paramètres de l'algorithme de recuit
    double T = 250; // température initiale
    double Tmin = 0.8; // température minimale
    double alpha = 0.99; // taux de refroidissement
    
    double T_max = 0;

    std::pair<double, double> T_init = init_T(tetravex);
    T = T_init.first;
    //Tmin = T_init.second;

    //T = T_max > 0 ? T_max : init_T(tetravex);

    int size = tetravex.getSize();
    int cost = calculateCost(tetravex.getTiles(), size);

    //std::random_device rd;
    //auto generator = std::mt19937(rd());
    //auto distribution = std::uniform_real_distribution<double>(0.0, 1.0);
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
            double r = (double)rand() / RAND_MAX;
            if (r < p)
            {
                cost = newCost;
                tetravex.setTiles(proposed);
            }
        }
        if (cost == 0)
            break;
        
        if (T > Tmin)
			T *= alpha;
		else
			T = Tmin;
    }

    return 1;
}