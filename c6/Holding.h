#ifndef Holding_h
#define Holding_h

#include <vector>
#include "PurchaseRecord.h"
#include <numeric>

class Holding {
public:
	void Add(PurchaseRecord& record) {
		purchaseRecords_.push_back(record);
	}

	std::vector<PurchaseRecord> Purchases() const {
		return purchaseRecords_;
	}

	unsigned int Shares() const {
		return accumulate(purchaseRecords_.begin(), purchaseRecords_.end(), 0,
			[] (int total, PurchaseRecord record) {
				return total + record.Shares; });
	}

private:
	std::vector<PurchaseRecord> purchaseRecords_;
};

#endif
