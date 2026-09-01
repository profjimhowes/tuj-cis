typedef struct {
    char *bases;
    size_t num_bases;
} Chromosome;

typedef enum { X, Y } AllosomeType;

typedef struct {
    AllosomeType type;
    union {
        Chromosome *x;
        Chromosome *y;
    } as;
} Allosome;

typedef struct {
    Chromosome *autosomes[];
    Allosome *sex_chromosome;
    size_t num_autosomes;
} HaploidGenome;

typedef struct {
    HaploidGenome *paternal, *maternal;
} DiploidGenome;

DiploidGenome *genome_from_file(const char *filename);
void genome_to_file(const DiploidGenome *genome, const char *filename);

char *genome_compress(const DiploidGenome *genome);
DiploidGenome *genome_decompress(const DiploidGenome *genome);

void recombine(Chromosome *left, Chromosome *right);
HaploidGenome *meiosis(const DiploidGenome *genome);
DiploidGenome *genome_reproduce(DiploidGenome *parent1, DiploidGenome *parent2);

typedef struct {
    size_t start, length;
} Match;

Match *matching_sequences(Chromosome *left, Chromosome *right, size_t min_length);
double genome_similarity(DiploidGenome *left, DiploidGenome *right);