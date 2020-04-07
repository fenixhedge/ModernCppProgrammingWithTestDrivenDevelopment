#include "Portfolio.h"
#include <numeric>

using namespace std;
using namespace boost::gregorian;

bool Portfolio::IsEmpty() const {
	//return 0 == purchaseRecords_.size();
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
	AddPurchaseRecord(symbol, shareChange, transactionDate);
}

void Portfolio::ThrowIfShareCountIsZero(int shareChange) const {
	if (shareChange == 0) {
		throw SharesCannotBeZeroException{};
	}
}

void Portfolio::AddPurchaseRecord(const string& symbol, int shareChange, const date& date) {
	if (!ContainSymbol(symbol)) {
		InitializePurchaseRecords(symbol);
	}
	Add(symbol, PurchaseRecord{shareChange, date});
}

void Portfolio::InitializePurchaseRecords(const string& symbol) {
	purchaseRecords_[symbol] = vector<PurchaseRecord>{};
	holdings_[symbol] = Holding{};
}
void Portfolio::Add(const string& symbol, PurchaseRecord&& record) {
	purchaseRecords_[symbol].push_back(record);
	holdings_[symbol].Add(record);
}

bool Portfolio::ContainSymbol(const string& symbol) const {
	//return purchaseRecords_.find(symbol) != purchaseRecords_.end();
	return holdings_.find(symbol) != holdings_.end();
}

unsigned int Portfolio::Shares(const std::string& symbol) const {
	//auto records = Find<vector<PurchaseRecord>>(purchaseRecords_, symbol);
	//return accumulate(records.begin(), records.end(), 0,
	//	[](int total, PurchaseRecord record) {
	//		return total + record.Shares; });
	return Find<Holding>(holdings_, symbol).Shares();
}

vector<PurchaseRecord> Portfolio::Purchases(const string& symbol) const {
	//return Find<vector<PurchaseRecord>>(purchaseRecords_, symbol);
	return Find<Holding>(holdings_, symbol).Purchases();
}
