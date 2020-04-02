#ifndef PlaceDescriptionService_h
#define PlaceDescriptionService_h

#include <string>

#include "Address.h"

class Http;

class PlaceDescriptionService {
public:
	PlaceDescriptionService(Http* http);
	std::string summaryDescription(const std::string& latitude, const std::string& longitude);

private:
	std::string createGetUrlRequest(const std::string& latitude, const std::string& longitude) const;
	std::string summaryDescription(const std::string& response) const;
	std::string keyValue(const std::string& key, const std::string& value) const;
	std::string get(const std::string& request) const;

	Http* http_;
};


#endif
