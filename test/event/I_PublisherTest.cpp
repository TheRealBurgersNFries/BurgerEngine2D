
#include <cstdint>
#include <memory>

#include "EventType.hpp"
#include "I_Publisher.hpp"

#include <CppUTest/TestHarness.h>

class TestPublisher : public BurgerEngine2D::I_Publisher {
    public:
        bool publishCalled = false;
        bool receivedNullEvent = false;
        BurgerEngine2D::EventType receivedType = 0;

        void Publish(
            const std::unique_ptr<BurgerEngine2D::EventType>& event
        ) override {
            publishCalled = true;
            receivedNullEvent = (event == nullptr);

            if (event != nullptr) {
                receivedType = *event;
            }
        }
};

TEST_GROUP(PublisherTestGroup) {
};

TEST(PublisherTestGroup, PublishReceivesTheEventType) {
    TestPublisher publisher;
    const std::unique_ptr<BurgerEngine2D::EventType> event =
        std::make_unique<BurgerEngine2D::EventType>(42);

    publisher.Publish(event);

    CHECK(publisher.publishCalled);
    CHECK(!publisher.receivedNullEvent);
    CHECK(publisher.receivedType == 42);
}

TEST(PublisherTestGroup, PublishCanReceiveNullPointer) {
    TestPublisher publisher;
    const std::unique_ptr<BurgerEngine2D::EventType> event;

    publisher.Publish(event);

    CHECK(publisher.publishCalled);
    CHECK(publisher.receivedNullEvent);
}
