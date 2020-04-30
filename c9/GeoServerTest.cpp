#include "GeoServer.h"

#include <condition_variable>
#include "TestTimer.h"

#include "CppUTestExtensions.h"
#include "CppUTest/TestHarness.h"

using namespace std;
using namespace std::chrono;

TEST_GROUP(AGeoServer) {
   GeoServer server;

   const string aUser{"auser"};
   const double LocationTolerance{0.005};
};

TEST(AGeoServer, TracksAUser) {
   server.track(aUser);

   CHECK_TRUE(server.isTracking(aUser));
}

TEST(AGeoServer, IsNotTrackingAUserNotTracked) {
   CHECK_FALSE(server.isTracking(aUser));
}

TEST(AGeoServer, TracksMultipleUsers) {
   server.track(aUser);
   server.track("anotheruser");

   CHECK_FALSE(server.isTracking("thirduser"));
   CHECK_TRUE(server.isTracking(aUser));
   CHECK_TRUE(server.isTracking("anotheruser"));
}

TEST(AGeoServer, IsTrackingAnswersFalseWhenUserNoLongerTracked) {
   server.track(aUser);
   
   server.stopTracking(aUser);

   CHECK_FALSE(server.isTracking(aUser));
}

TEST(AGeoServer, UpdatesLocationOfUser) {
   server.track(aUser);

   server.updateLocation(aUser, Location{38, -104});

   auto location = server.locationOf(aUser);
   DOUBLES_EQUAL(38, location.latitude(), LocationTolerance);
   DOUBLES_EQUAL(-104, location.longitude(), LocationTolerance);
}

TEST(AGeoServer, AnswersUnknownLocationForUserNotTracked) {
   CHECK_TRUE(server.locationOf("anAbUser").isUnknown());
}

TEST(AGeoServer, AnswersUnknownLocationForTrackedUserWithNoLocationUpdate) {
   server.track(aUser);

   CHECK_TRUE(server.locationOf(aUser).isUnknown());
}

TEST(AGeoServer, AnswersUnknownLocationForUserNoLongerTracked) {
   server.track(aUser);
   server.updateLocation(aUser, Location(40, 100));

   server.stopTracking(aUser);

   CHECK_TRUE(server.locationOf(aUser).isUnknown());
}

class GeoServerUsersInBoxTests : public Utest {
public:
   GeoServer server;

   const double TenMeters { 10 };
   const double Width { 2000 + TenMeters };
   const double Height { 4000 + TenMeters };
   const string aUser { "auser" };
   const string bUser { "buser" };
   const string cUser { "cuser" };

   Location aUserLocation { 38, -103 };

   shared_ptr<ThreadPool> pool;

   void setup() override {
      server.useThreadPool(pool);

      server.track(aUser);
      server.track(bUser);
      server.track(cUser);

      server.updateLocation(aUser, aUserLocation);
   }

   void addUsersAt(unsigned number, const Location& location) {
      for (unsigned i{0}; i < number; i++) {
         string user{"user" + to_string(i)};
         server.track(user);
         server.updateLocation(user, location);
      }
   }
};

TEST_GROUP_BASE(AGeoServer_UsersInBox, GeoServerUsersInBoxTests) {
   class GeoServerUserTrackingListener : public GeoServerListener {
   public:
      void updated(const User& user) { Users.push_back(user); }
      vector<User> Users;
   } trackingListener;

   class SingleThreadedPool : public ThreadPool {
   public:
      virtual void add(Work work) override { work.execute(); }
   };

   void setup() override {
      pool = make_shared<SingleThreadedPool>();
      GeoServerUsersInBoxTests::setup();
   }

   vector<string> UserNames(const vector<User>& users) {
      return Collect<User, string>(users, [](User each) { return each.name(); });
   }
};

TEST(AGeoServer_UsersInBox, AnswersUsersInSpecifiedRange) {
   pool->start(0);
   server.updateLocation(
      bUser, Location{ aUserLocation.go(Width / 2 - TenMeters, East)});

   server.usersInBox(aUser, Width, Height, &trackingListener);

   CHECK_EQUAL(vector<string>{ bUser }, UserNames(trackingListener.Users));
}

TEST(AGeoServer_UsersInBox, AnswersOnlyUsersInSpecifiedRange) {
   pool->start(0);
   server.updateLocation(
      bUser, Location{ aUserLocation.go(Width / 2 + TenMeters, East) });
   server.updateLocation(
      cUser, Location{ aUserLocation.go(Width / 2 - TenMeters, East) });

   server.usersInBox(aUser, Width, Height, &trackingListener);

   CHECK_EQUAL(vector<string>{ cUser }, UserNames(trackingListener.Users));
}

TEST(AGeoServer_UsersInBox, HandlesLargeNumbersOfUsers) {
   pool->start(0);
   const unsigned lots { 500000 };
   addUsersAt(lots, Location{aUserLocation.go(TenMeters, West)});

   TestTimer timer;
   server.usersInBox(aUser, Width, Height, &trackingListener);

   CHECK_EQUAL(lots, trackingListener.Users.size());
}
