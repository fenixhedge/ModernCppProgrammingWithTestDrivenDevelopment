#ifndef Portfolio_h
#define Portfolio_h

#include <string>
#include <exception>

class InvalidPurchaseException : public std::exception {
};

class Portfolio {
public:
	Portfolio();
	bool IsEmpty() const;
	void Purchase(const std::string& symbol, unsigned int shareCount);
	unsigned int ShareCount(const std::string& symbol) const;

private:
	unsigned int shareCount_;
};

#endif
