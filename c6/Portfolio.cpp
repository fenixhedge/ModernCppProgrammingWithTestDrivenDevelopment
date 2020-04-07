#include "Portfolio.h"

using namespace std;
using namespace boost::gregorian;

const date Portfolio::FIXED_PURCHASE_DATE(date(2014, Jan, 1));

bool Portfolio::IsEmpty() const {
	return 0 == holdings_.size();
}

void Portfolio::Purchase(
		const std::string& symbol, unsigned int shares, const date& transactionDate) {
	if (shares == 0) {
		throw InvalidPurchaseException{};
	}
	holdings_[symbol] = Shares(symbol) + shares;
	purchases_.push_back(PurchaseRecord(shares, transactionDate));
}

void Portfolio::Sell(
		const string& symbol, unsigned int shares, const date& transactionDate) {
	if (Shares(symbol) < shares) {
		throw InvalidSellException{};
	}
	holdings_[symbol] = Shares(symbol) - shares;
	purchases_.push_back(PurchaseRecord(-shares, transactionDate));
}

unsigned int Portfolio::Shares(const std::string& symbol) const {
	auto it = holdings_.find(symbol);
	if (it == holdings_.end()) return 0;
	return it->second;
}

vector<PurchaseRecord> Portfolio::Purchases(const string& symbol) const {
	return purchases_;
}
