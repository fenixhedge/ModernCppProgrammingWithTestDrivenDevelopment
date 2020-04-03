#ifndef PlaceDescriptionService_h
#define PlaceDescriptionService_h

#include <string>
#include "Address.h"
#include "AddressExtractor.h"

template<typename HTTP>
class PlaceDescriptionServiceTemplate {
public:
	std::string summaryDescription(const std::string& latitude, const std::string& longitude) {
		auto request = createGetUrlRequest(latitude, longitude);
		auto response = get(request);
		return summaryDescription(response);
	}

	//mocks in tests needs this reference
	HTTP& http() {
		return http_;
	}

private:
	std::string summaryDescription(const std::string& response) const {
		AddressExtractor extractor;
		auto address = extractor.addressFrom(response);
		return address.summaryDescription();
	}

	std::string createGetUrlRequest(const std::string& latitude, const std::string& longitude) const {
		std::string server{"http://open.mapquestapi.com/"};
		std::string document{"nominatim/v1/reverse"};
		return server + document + "?" +
			keyValue("format", "json") + "&" +
			keyValue("lat", latitude) + "&" +
			keyValue("lon", longitude);
	}

	std::string get(const std::string& request) {
		http_.initialize();
		return http_.get(request);
	}

	std::string keyValue(const std::string& key, const std::string& value) const {
		return key + "=" + value;
	}

	HTTP http_;
};

class Http;
typedef PlaceDescriptionServiceTemplate<Http> PlaceDescriptionService;

#endif
