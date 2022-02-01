#ifndef NEAT_GENE_H
#define NEAT_GENE_H

/**
 * Class containing gene information.
 */
class Gene {
public:
    int in;
    int out;
    int innovation;
    bool enabled;
    double weight;

    Gene();

    Gene(const Gene &gene);

    Gene(int in, int out, int innovation, bool enabled, double weight);

    Gene &operator=(const Gene &gene);
};

#endif
