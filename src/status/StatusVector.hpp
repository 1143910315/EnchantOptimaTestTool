#pragma once
#include "Status.hpp"
#include <vector>
template<typename T>
class StatusVector : public std::vector<Status<T>> {
};
