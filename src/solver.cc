#include "solver.hh"

// fonction pour calculer la différence d'énergie entre la configuration
// actuelle et la configuration proposée
double energyDifference(Tile current[], Tile proposed[], int size)
{
    // calculer la différence d'énergie en comparant les bords des tuiles
    double difference = 0;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (i > 0)
            {
                difference += abs(current[i * size + j].north
                                  - current[(i - 1) * size + j].south);
                difference -= abs(proposed[i * size + j].north
                                  - proposed[(i - 1) * size + j].south);
            }
            if (j > 0)
            {
                difference += abs(current[i * size + j].west
                                  - current[i * size + j - 1].east);
                difference -= abs(proposed[i * size + j].west
                                  - proposed[i * size + j - 1].east);
            }
        }
    }
    return difference;
}

// fonction pour effectuer une transition aléatoire entre les configurations
void randomTransition(std::vector<Tile> tiles, int size)
{
    // Choisir deux axes aléatoires
    int axis1 = rand() % size;
    int axis2 = rand() % size;
    while (axis2 == axis1)
        axis2 = rand() % size;
}

bool checkSolution(Tile tiles[], int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (i > 0)
            {
                // vérifier les bords nord-sud des tuiles
                if (tiles[i * size + j].north
                    != tiles[(i - 1) * size + j].south)
                {
                    return false;
                }
            }
            if (j > 0)
            {
                // vérifier les bords est-ouest des tuiles
                if (tiles[i * size + j].west != tiles[i * size + j - 1].east)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

// calcule le coût actuel de la grille en comptant le nombre de cases mal
// placées
int calculateCost(std::vector<Tile> tiles, int size)
{
    int c = 0;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (i > 0)
                if (tiles[i * size + j].north
                    != tiles[(i - 1) * size + j].south)
                    c++;
            if (j > 0)
                if (tiles[i * size + j].west != tiles[i * size + j - 1].east)
                    c++;
        }
    }
    return c;
}

// fonction pour résoudre le tetravex en utilisant l'algorithme de recuit de
// Metropolis-Hastings
void solveTetravex(std::vector<Tile> tiles, int size)
{
    srand(time(NULL));

    // paramètres de l'algorithme de recuit
    double T = 100000; // température initiale
    double Tmin = 1e-6; // température minimale
    double alpha = 0.99; // taux de refroidissement

    int cost = calculateCost(tiles, size);

    while (cost)
    {
        std::vector<Tile> proposed = std::vector<Tile>(size * size);
        for (int j = 0; j < size * size; j++)
        {
            proposed[j] = tiles[j];
        }
        randomTransition(proposed, size);
        int newCost = calculateCost(proposed, size);
        int delta = newCost - cost;
        if (delta < 0)
        {
            cost = newCost;
            for (int j = 0; j < size * size; j++)
            {
                tiles[j] = proposed[j];
            }
        }
        else if (rand() < exp(-delta / T))
        {
            cost = newCost;
            for (int j = 0; j < size * size; j++)
            {
                tiles[j] = proposed[j];
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

