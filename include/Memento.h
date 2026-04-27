#ifndef MEMENTO_H
#define MEMENTO_H

#include <vector>

class Memento {
private:
    std::vector<std::vector<char>> snapshot;

public:
    explicit Memento(const std::vector<std::vector<char>>& grid);
    
    std::vector<std::vector<char>> restore() const;
};

#endif 