#include "Portfolio.h"

using namespace std;
using namespace boost::gregorian;

bool Portfolio::IsEmpty() const {
	return 0 == holdings_.size();
}

void Portfolio::Purchase(
		const string& symbol, unsigned int shares, const date& transactionDate) {
	Transact(symbol, shares, transactionDate);
}

void Portfolio::Sell(
		const string& symbol, unsigned int shares, const date& transactionDate) {
	if (Shares(symbol) < shares) {
		throw InvalidSellException{};
	}
	Transact(symbol, -shares, transactionDate);
}

void Portfolio::Transact(
		const string& symbol, int shareChange, const date& transactionDate) {
	ThrowIfShareCountIsZero(shareChange);
	UpdateShares(symbol, shareChange);
	AddPurchaseRecord(symbol, shareChange, transactionDate);
}

void Portfolio::ThrowIfShareCountIsZero(int shareChange) const {
	if (shareChange == 0) {
		throw SharesCannotBeZeroException{};
	}
}

void Portfolio::UpdateShares(const string& symbol, int shareChange) {
	holdings_[symbol] = Shares(symbol) + shareChange;
}

void Portfolio::AddPurchaseRecord(const string& symbol, int shareChange, const date& date) {
	purchases_.push_back(PurchaseRecord(shareChange, date));
	auto it = purchaseRecords_.find(symbol);
	if (it == purchaseRecords_.end()) {
		purchaseRecords_[symbol] = vector<PurchaseRecord>{};
	}
	purchaseRecords_[symbol].push_back(PurchaseRecord{shareChange, date});
}

unsigned int Portfolio::Shares(const std::string& symbol) const {
	auto it = holdings_.find(symbol);
	if (it == holdings_.end()) return 0;
	return it->second;
}

vector<PurchaseRecord> Portfolio::Purchases(const string& symbol) const {
	return purchaseRecords_.find(symbol)->second;
}
