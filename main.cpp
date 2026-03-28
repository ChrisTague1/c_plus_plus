#include <iostream>
#include <map>

enum class Side {
    Bid,
    Ask
};

using OrderId = uint64_t;
using Volume = int64_t;
using Price = int64_t;

void AddOrder(OrderId orderId, Side side, Price price, Volume volume);
void ModifyOrder(OrderId orderId, Volume newVolume);
void DeleteOrder(OrderId orderId);

template <class T>
typename T::iterator AddOrder(T& levels, Price price, Volume volume) {
    auto [it, inserted] = levels.try_emplace(price, volume);

    if (inserted == false)
        it->second += volume;
    
    return it;
}

template <class T>
void DeleteOrder(typename T::iterator it, T& levels, Price price, Volume volume) {
    it->second -= volume;
    if (it->second <= 0)
        levels.erase(it);
}

int main() {
    std::cout << "Hello there." << std::endl;

    return 0;
}