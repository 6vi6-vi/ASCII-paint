#include "Memento.h"

Memento::Memento(const std::vector<std::vector<char>>& grid) : snapshot(grid) {}

std::vector<std::vector<char>> Memento::restore() const {
    return snapshot;
}