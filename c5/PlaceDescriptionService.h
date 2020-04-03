#ifndef PlaceDescriptionService_h
#define PlaceDescriptionService_h

#include <string>
#include <memory>

#include "Address.h"

class HttpFactory;

class PlaceDescriptionService {
public:
	PlaceDescriptionService(std::shared_ptr<HttpFactory> httpFactory);
	std::string summaryDescription(const std::string& latitude, const std::string& longitude);

private:
	std::string createGetUrlRequest(const std::string& latitude, const std::string& longitude) const;
	std::string summaryDescription(const std::string& response) const;
	std::string keyValue(const std::string& key, const std::string& value) const;
	std::string get(const std::string& request) const;

	std::shared_ptr<HttpFactory> httpFactory_;
};


#endif
