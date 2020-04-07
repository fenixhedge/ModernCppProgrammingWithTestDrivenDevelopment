#include "gmock/gmock.h"
#include "Portfolio.h"
#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace std;
using namespace ::testing;
using namespace boost::gregorian;

class APortfolio : public Test {
public:
	static const string IBM;
	static const string SAMSUNG;
	Portfolio portfolio_;
	static const date ArbitraryDate;

	void Purchase(
			const string& symbol,
			unsigned int shares,
			const date& transactionDate=APortfolio::ArbitraryDate) {
		portfolio_.Purchase(symbol, shares, transactionDate);
	}	
};
const string APortfolio::IBM{"IBM"};
const string APortfolio::SAMSUNG{"SAMSUNG"};
const date APortfolio::ArbitraryDate{2014, Sep, 5};

TEST_F(APortfolio, IsEmptyWhenCreated) {
	ASSERT_TRUE(portfolio_.IsEmpty());
}

TEST_F(APortfolio, IsNotEmptyAfterPurchase) {
	Purchase(IBM, 1);
	ASSERT_FALSE(portfolio_.IsEmpty());
}

TEST_F(APortfolio, AnswersZeroForShareCountOfUnpurchasedSymbol) {
	ASSERT_THAT(portfolio_.Shares("AAPL"), Eq(0u));
}

TEST_F(APortfolio, AnswersSharesForPurchasedSymbol) {
	Purchase(IBM, 2);
	ASSERT_THAT(portfolio_.Shares(IBM), Eq(2u));
}

TEST_F(APortfolio, ThrowsOnPurchaseOfZeroShares) {
	ASSERT_THROW(Purchase(IBM, 0), InvalidPurchaseException);
}

TEST_F(APortfolio, AnswersSharesForAppropriateSymbol) {
	Purchase(IBM, 5);
	Purchase(SAMSUNG, 10);

	ASSERT_THAT(portfolio_.Shares(IBM), Eq(5u));
}

TEST_F(APortfolio, SharesReflectsAccumulatedPurchasesOfSameSymbol) {
	Purchase(IBM, 5u);
	Purchase(IBM, 15u);

	ASSERT_THAT(portfolio_.Shares(IBM), Eq(5u + 15));
}

TEST_F(APortfolio, ReducesSharesOfSymbolOnSell) {
	Purchase(SAMSUNG, 30);
	portfolio_.Sell(SAMSUNG, 13);

	ASSERT_THAT(portfolio_.Shares(SAMSUNG), Eq(30u - 13));
}

TEST_F(APortfolio, ThrowsWhenSellingMoreThanPurchased) {
	ASSERT_THROW(portfolio_.Sell(SAMSUNG, 1u), InvalidSellException);
}

TEST_F(APortfolio, AnswersThePurchaseRecordForASinglePurchase) {
	date dateOfPurchase(2014, Mar, 17);
	Purchase(SAMSUNG, 5, dateOfPurchase);

	auto purchases = portfolio_.Purchases(SAMSUNG);

	auto purchase = purchases[0];
	ASSERT_THAT(purchase.Shares, Eq(5));
	ASSERT_THAT(purchase.Date, Eq(dateOfPurchase));
}

TEST_F(APortfolio, IncludesSalesInPurchaseRecords) {
	Purchase(SAMSUNG, 10);
	portfolio_.Sell(SAMSUNG, 5, ArbitraryDate);

	auto purchases = portfolio_.Purchases(SAMSUNG);
	auto purchase = purchases[1];
	ASSERT_THAT(purchase.Shares, Eq(-5));
	ASSERT_THAT(purchase.Date, Eq(ArbitraryDate));
}
