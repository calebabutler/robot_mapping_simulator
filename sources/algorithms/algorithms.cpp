
// Includes
#include "algorithms.h"
#include "random_algorithm.h"
#include "no_backtrack_random_algorithm.h"

void add_algorithms(Application& app)
{
    add_random_algorithm(app);
    add_no_backtrack_random_algorithm(app);
}
