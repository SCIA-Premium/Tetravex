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
        }
    }
    return cost;
}

// fonction pour effectuer une transition aléatoire entre les configurations
void randomTransition(std::vector<Tile> &tiles, int size)
{
    int i = rand() % (size * size);
    while (tiles[i].inPlace)
        i = rand() % (size * size);
    
    int j = rand() % (size * size);
    while (tiles[j].inPlace && i == j)
        j = rand() % (size * size);

    std::swap(tiles[i], tiles[j]);
}

// fonction pour résoudre le tetravex en utilisant l'algorithme de recuit de
// Metropolis-Hastings
void solveTetravex(Tetravex &tetravex)
{
    srand(time(NULL));

    // paramètres de l'algorithme de recuit
    double T = 1000000; // température initiale
    double Tmin = 1e-6; // température minimale
    double alpha = 0.99; // taux de refroidissement

    int size = tetravex.getSize();
    int cost = calculateCost(tetravex.getTiles(), size);
    while (cost)
    {
        std::vector<Tile> proposed = std::vector<Tile>(tetravex.getTiles());
        randomTransition(proposed, size);
        int newCost = calculateCost(proposed, size);
        int delta = newCost - cost;
        if (delta < 0)
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
        T *= alpha;
    }
    /*for (int i = 0; i < NUM_ITERATIONS; i++)
    {
        // effectuer une transition aléatoire
        Tile proposed[size * size];
        for (int j = 0; j < size * size; j++)
        {
            proposed[j] = tiles[j];
        }
        randomTransition(proposed, size);
        // calculer la différence d'énergie entre la configuration actuelle et
        // la configuration proposée
        double deltaE = energyDifference(tiles, proposed, size);

        // décider si la transition doit être acceptée ou rejetée
        if (deltaE < 0)
        {
            // acceptation de la transition car l'énergie est plus basse
            for (int j = 0; j < size * size; j++)
            {
                tiles[j] = proposed[j];
            }
        }
        else
        {
            // rejet de la transition car l'énergie est plus élevée
            // calculer la probabilité de transition
            double p = exp(-deltaE);
            // générer un nombre aléatoire entre 0 et 1
            double r = (double)rand() / (double)RAND_MAX;


            // acceptation de la transition avec une probabilité p
            if (r < p)
            {
                for (int j = 0; j < size * size; j++)
                {
                    tiles[j] = proposed[j];
                }
            }
        }

        if (checkSolution(tiles, size))
            break;

    }*/
}

