#include <gtest/gtest.h>
#include "../config/config.h"
#include "../database/database.h"
#include "../database/person.h"
#include <Poco/Data/SessionFactory.h>

using Poco::Data::Session;
using Poco::Data::Statement;
using namespace Poco::Data::Keywords;

class TestApp : public ::testing::Test {
protected:
    TestApp() {
        Config::get().host() = "127.0.0.1";
        Config::get().database() = "itprojects_labs_test";
        Config::get().port() = "3306";
        Config::get().login() = "test";
        Config::get().password() = "pzjqUkMnc7vfNHET";
    }
    ~TestApp() {
        Poco::Data::Session session = database::Database::get().create_session();
        Statement drop(session);
        drop << "DELETE FROM Person", now;
        Statement reset_ai(session);
        reset_ai << "ALTER TABLE Person AUTO_INCREMENT = 1", now;
    }
     void SetUp() {}
     void TearDown() {}

protected:
};

TEST_F(TestApp, TestPerson) {

    database::Person person;
    person.login() = "alex-12345";
    person.first_name() = "Alex";
    person.last_name() = "Vinnikov";
    person.age() = 22;

    testing::internal::CaptureStdout();
    person.save_to_mysql();
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "inserted:1\n");

    person.login() = "alex-12345_2";
    person.first_name() = "Alexey";
    testing::internal::CaptureStdout();
    person.save_to_mysql();
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "inserted:2\n");
    person.login() = "alex";
    person.last_name() = "Vinn";
    testing::internal::CaptureStdout();
    person.save_to_mysql();
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "inserted:3\n");

    database::Person result = database::Person::read_by_login("alex-12345");
    ASSERT_EQ(result.get_first_name(), "Alex");
    ASSERT_EQ(result.get_last_name(), "Vinnikov");

    database::Person result2 = database::Person::read_by_login("alex");
    ASSERT_EQ(result2.get_first_name(), "Alexey");
    ASSERT_EQ(result2.get_last_name(), "Vinn");

    auto results = database::Person::read_all();
    ASSERT_EQ(results.size(), 3);
    ASSERT_EQ(results.at(0).get_first_name(), "Alex");

    auto results2 = database::Person::search("Ale", "Vin");
    ASSERT_EQ(results2.size(), 3);
    ASSERT_EQ(results2.at(0).get_first_name(), "Alex");

    auto results3 = database::Person::search("Alexe", "Vinnikov");
    ASSERT_EQ(results3.size(), 1);
    ASSERT_EQ(results3.at(0).get_login(), "alex-12345_2");

}


int main(int argc, char **argv) {

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}