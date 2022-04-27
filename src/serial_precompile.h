#pragma once

#include <string>
#include <vector>

struct Article {
    std::string name;
    std::vector< Article > refs;
};
