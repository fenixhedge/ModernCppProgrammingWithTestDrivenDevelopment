#include "Portfolio.h"

using namespace std;

Portfolio::Portfolio() : shareCount_{0u} {
}

bool Portfolio::IsEmpty() const {
	return 0 == shareCount_;
}

void Portfolio::Purchase(const std::string& symbol, unsigned int shareCount) {
	if (shareCount <= 0) {
		throw InvalidPurchaseException{};
	}
	shareCount_ = shareCount;
}

unsigned int Portfolio::ShareCount(const std::string& symbol) const {
	return shareCount_;
}
