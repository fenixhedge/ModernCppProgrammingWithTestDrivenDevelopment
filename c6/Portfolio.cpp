#include "Portfolio.h"

using namespace std;

bool Portfolio::IsEmpty() const {
	return 0 == holdings_.size();
}

void Portfolio::Purchase(const std::string& symbol, unsigned int shares) {
	if (shares == 0) {
		throw InvalidPurchaseException{};
	}
	holdings_[symbol] = Shares(symbol) + shares;
}

void Portfolio::Sell(const string& symbol, unsigned int shares) {
	holdings_[symbol] = Shares(symbol) - shares;
}

unsigned int Portfolio::Shares(const std::string& symbol) const {
	auto it = holdings_.find(symbol);
	if (it == holdings_.end()) return 0;
	return it->second;
}
