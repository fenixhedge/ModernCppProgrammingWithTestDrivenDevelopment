#include "PlaceDescriptionService.h"
#include "Http.h"
#include "CurlHttp.h"
#include "AddressExtractor.h"
#include <string>

using namespace std;

PlaceDescriptionService::PlaceDescriptionService()
	: http_(make_shared<CurlHttp>()) {}

string PlaceDescriptionService::summaryDescription(
		const string& latitude, const string& longitude) {
	auto request = createGetUrlRequest(latitude, longitude);
	auto response = get(request);
	return summaryDescription(response);
}

string PlaceDescriptionService::summaryDescription(const string& response) const {
	AddressExtractor extractor;
	auto address = extractor.addressFrom(response);
	return address.summaryDescription();
}

string PlaceDescriptionService::get(const std::string& request) const {
	auto http = httpService();
	http->initialize();
	return http->get(request);
}

shared_ptr<Http> PlaceDescriptionService::httpService() const {
	return http_;
}

string PlaceDescriptionService::createGetUrlRequest(
		const string& latitude, const string& longitude) const {
	string server{"http://open.mapquestapi.com/"};
	string document{"nominatim/v1/reverse"};
	return server + document + "?" +
		keyValue("format", "json") + "&" +
		keyValue("lat", latitude) + "&" +
		keyValue("lon", longitude);
}

string PlaceDescriptionService::keyValue(
		const string& key, const string& value) const {
	return key + "=" + value;
}
