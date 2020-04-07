#ifndef Portfolio_h
#define Portfolio_h

#include <string>
#include <exception>
#include <unordered_map>
#include <vector>
#include <boost/date_time/gregorian/gregorian.hpp>

class SharesCannotBeZeroException : public std::exception {
};

class InvalidSellException : public std::exception {
};

struct PurchaseRecord {
	PurchaseRecord(int shares, const boost::gregorian::date& date)
		: Shares(shares)
		, Date(date) {
	}

	int Shares;
	boost::gregorian::date Date;
};

class Portfolio {
public:
	bool IsEmpty() const;

	void Purchase(
		const std::string& symbol,
		unsigned int shares,
		const boost::gregorian::date& transactionDate);
	void Sell(
		const std::string& symbol,
		unsigned int shares,
		const boost::gregorian::date& transactionDate);

	unsigned int Shares(const std::string& symbol) const;
	std::vector<PurchaseRecord> Purchases(const std::string& symbol) const;

private:
	void Transact(const std::string& symbol, int shares, const boost::gregorian::date& transactionDate);
	void ThrowIfShareCountIsZero(int shareChange) const;
	void UpdateShares(const std::string& symbol, int shareChange);
	void AddPurchaseRecord(int shareChange, const boost::gregorian::date& date);

	std::unordered_map<std::string, unsigned int> holdings_;
	std::vector<PurchaseRecord> purchases_;
};

#endif
