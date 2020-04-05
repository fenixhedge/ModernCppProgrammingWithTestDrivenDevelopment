#include "gmock/gmock.h"
#include "Portfolio.h"
#include <string>

using namespace std;
using namespace ::testing;

class APortfolio : public Test {
public:
	static const string IBM;
	static const string SAMSUNG;
	Portfolio portfolio_;
};
const string APortfolio::IBM{"IBM"};
const string APortfolio::SAMSUNG{"SAMSUNG"};

TEST_F(APortfolio, IsEmptyWhenCreated) {
	ASSERT_TRUE(portfolio_.IsEmpty());
}

TEST_F(APortfolio, IsNotEmptyAfterPurchase) {
	portfolio_.Purchase(IBM, 1);
	ASSERT_FALSE(portfolio_.IsEmpty());
}

TEST_F(APortfolio, AnswersZeroForShareCountOfUnpurchasedSymbol) {
	ASSERT_THAT(portfolio_.Shares("AAPL"), Eq(0u));
}

TEST_F(APortfolio, AnswersShareCountForPurchasedSymbol) {
	portfolio_.Purchase(IBM, 2);
	ASSERT_THAT(portfolio_.Shares(IBM), Eq(2u));
}

TEST_F(APortfolio, ThrowsOnPurchaseOfZeroShares) {
	ASSERT_THROW(portfolio_.Purchase(IBM, 0), InvalidPurchaseException);
}

TEST_F(APortfolio, AnswersSharesForAppropriateSymbol) {
	portfolio_.Purchase(IBM, 5);
	portfolio_.Purchase(SAMSUNG, 10);

	ASSERT_THAT(portfolio_.Shares(IBM), Eq(5u));
}

TEST_F(APortfolio, SharesReflectsAccumulatedPurchasesOfSameSymbol) {
	portfolio_.Purchase(IBM, 5u);
	portfolio_.Purchase(IBM, 15u);

	ASSERT_THAT(portfolio_.Shares(IBM), Eq(5u + 15));
}

TEST_F(APortfolio, ReducesSharesOfSymbolOnSell) {
	portfolio_.Purchase(SAMSUNG, 30);
	portfolio_.Sell(SAMSUNG, 13);

	ASSERT_THAT(portfolio_.Shares(SAMSUNG), Eq(30u - 13));
}

TEST_F(APortfolio, ThrowsWhenSellingMoreThanPurchased) {
	ASSERT_THROW(portfolio_.Sell(SAMSUNG, 1u), InvalidSellException);
}
