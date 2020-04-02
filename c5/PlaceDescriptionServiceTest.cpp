#include "gmock/gmock.h"

#include "PlaceDescriptionService.h"
#include "Http.h"

using namespace testing;
using namespace std;

class APlaceDescriptionService : public Test {
public:
	static const string ValidLatitude;
	static const string ValidLongitude;
};

const string APlaceDescriptionService::ValidLatitude("38.005");
const string APlaceDescriptionService::ValidLongitude("-104.44");

class HttpStub : public Http {
public:
	string returnResponse;
	void initialize() override {}
	string get(const string& url) const override {
		verify(url);
		return returnResponse;
	};

	void verify(const string& url) const {
		string urlStart("http://open.mapquestapi.com/nominatim/v1/reverse?format=json&");
		string expectedArgs(urlStart +
				"lat=" + APlaceDescriptionService::ValidLatitude + "&" +
				"lon=" + APlaceDescriptionService::ValidLongitude);
		ASSERT_THAT(url, EndsWith(expectedArgs));
	}
};

TEST_F(APlaceDescriptionService, ReturnsDescriptionForValidLocation) {
	HttpStub httpStub;
	httpStub.returnResponse = R"({"address" : {
					"road" : "Drury Ln",
					"city" : "Fountain",
					"state" : "CO",
					"country" : "US" }})";
	PlaceDescriptionService service{&httpStub};

	auto description = service.summaryDescription(ValidLatitude, ValidLongitude);

	ASSERT_THAT(description, Eq("Drury Ln, Fountain, CO, US"));
}
