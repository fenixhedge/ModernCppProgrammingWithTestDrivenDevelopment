#ifndef AddressExtractor_h
#define AddressExtractor_h

#include <json/reader.h>
#include <json/value.h>
#include <string>
#include "Address.h"


class AddressExtractor {
public:
	Address addressFrom(const std::string& json) const {
		Address address;
		Json::Value jsonAddress{jsonAddressFrom(json)};
		if (jsonAddress != Json::nullValue) {
			populate(address, jsonAddress);
		}
		return address;
	}
private:
	Json::Value jsonAddressFrom(const std::string& json) const {
		Json::Value jsonAddress = parse(json);
		return jsonAddress["address"];
	}

	void populate(Address& address, const Json::Value& jsonAddress) const {
		address.road = jsonAddress["road"].asString();
		address.city = jsonAddress["city"].asString();
		address.state = jsonAddress["state"].asString();
		address.country = jsonAddress["country"].asString();
	}

	Json::Value parse(const std::string& json) const {
		Json::CharReaderBuilder builder;
		Json::Value root;
		JSONCPP_STRING err;
		const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
		reader->parse(json.c_str(), json.c_str() + json.length(), &root, &err);
		return root;
	}
};

#endif
