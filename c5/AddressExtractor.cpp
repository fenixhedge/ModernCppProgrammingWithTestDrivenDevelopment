#include "AddressExtractor.h"

using namespace std;
using namespace Json;

Address AddressExtractor::addressFrom(const string& json) const {
	Address address;
	Json::Value jsonAddress{jsonAddressFrom(json)};
	populate(address, jsonAddress);
	return address;
}

Value AddressExtractor::jsonAddressFrom(const string& json) const {
	Json::Value jsonAddress = parse(json);
	return jsonAddress["address"];
}

void AddressExtractor::populate(Address& address, const Value& jsonAddress) const {
	address.road = getString(jsonAddress, "road");
	address.city = getString(jsonAddress, "city");
	address.state = getString(jsonAddress, "state");
	address.country = getString(jsonAddress, "country");
}

Value AddressExtractor::parse(const string& json) const {
	Json::CharReaderBuilder builder;
	Json::Value root;
	JSONCPP_STRING err;
	const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
	reader->parse(json.c_str(), json.c_str() + json.length(), &root, &err);
	return root;
}

string AddressExtractor::getString(const Value& value, const string& name) const {
	return value[name].asString();
}
