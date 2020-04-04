#include "Portfolio.h"

using namespace std;

bool Portfolio::IsEmpty() const {
	return 0 == holdings_.size();
}

void Portfolio::Purchase(const std::string& symbol, unsigned int shareCount) {
	if (shareCount <= 0) {
		throw InvalidPurchaseException{};
	}
	holdings_[symbol] = shareCount;
}

unsigned int Portfolio::ShareCount(const std::string& symbol) const {
	auto it = holdings_.find(symbol);
	if (it == holdings_.end()) return 0;
	return it->second;
}
