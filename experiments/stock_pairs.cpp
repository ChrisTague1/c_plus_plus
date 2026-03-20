#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <optional>
#include <cmath>

class StockPairMonitor {
    double threshold;
    std::unordered_map<std::string, std::optional<double>> prices;
    std::unordered_map<std::string, std::unordered_set<std::string>> pairs;

    void notify(const std::string& a, double price_a, const std::string& b,
                double price_b) {
        std::cout << "ALERT: " << a << " (" << price_a << ") and " << b << " ("
                  << price_b << ") differ by " << std::abs(price_a - price_b)
                  << " (threshold: " << threshold << ")\n";
    }

   public:
    StockPairMonitor(double threshold) : threshold(threshold) {}

    void pair(const std::string& a, const std::string& b) {
        prices.try_emplace(a, std::nullopt);
        prices.try_emplace(b, std::nullopt);
        pairs[a].insert(b);
        pairs[b].insert(a);
    }

    void update(const std::string& stock, double price) {
        prices[stock] = price;

        // if (!pairs.contains(stock)) return;

        // for (const auto& pair : pairs[stock]) {
        //     auto& pair_price = prices[pair];

        //     if (
        //         pair_price.has_value() &&
        //         std::abs(price - pair_price.value()) > threshold
        //     ) {
        //         notify(stock, price, pair, pair_price.value());
        //     }
        // }

        auto it = pairs.find(stock);
        if (it == pairs.end()) return;

        for (const auto& partner : it->second) {
            auto& partner_price = prices[partner];
            if (partner_price.has_value() &&
                std::abs(price - *partner_price) > threshold) {
                notify(stock, price, partner, *partner_price);
            }
        }
    }
};

int main() {
    StockPairMonitor monitor(5.0);

    monitor.pair("AAPL", "MSFT");
    monitor.pair("AAPL", "GOOG");

    monitor.update("AAPL", 150.0);
    monitor.update("MSFT", 152.0);  // within threshold
    monitor.update("GOOG", 140.0);  // triggers alert with AAPL (diff=10)
    monitor.update("AAPL", 145.0);  // triggers alert with MSFT (diff=7)

    return 0;
}
