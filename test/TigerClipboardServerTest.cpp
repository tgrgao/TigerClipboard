#include "gtest/gtest.h"
#include <iostream>

#include "../src/TigerClipboardServer.h"

namespace {

// The fixture for testing class Foo.
class TigerClipboardServerTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  TigerClipboardServerTest() {
     // You can do set-up work for each test here.
  }

  ~TigerClipboardServerTest() override {
     // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Class members declared here can be used by all tests in the test suite
  // for Foo.
};

TEST_F(TigerClipboardServerTest, CheckInitializedValues) {
    TigerClipboardServer clipboardServer;
    clipboardServer.initServer();
    EXPECT_EQ(clipboardServer.status(), TigerClipboardServer::Status::OK);
    EXPECT_EQ(clipboardServer.copyMode(), TigerClipboardServer::CopyMode::COPY_STATIC);
    EXPECT_EQ(clipboardServer.pasteMode(), TigerClipboardServer::PasteMode::PASTE_STATIC);
    EXPECT_EQ(clipboardServer.clipboard().size(), 0);
    EXPECT_EQ(clipboardServer.copyIterator(), clipboardServer.clipboard().begin());
    EXPECT_EQ(clipboardServer.pasteIterator(), clipboardServer.clipboard().begin());
}

TEST_F(TigerClipboardServerTest, TestStaticCopy) {
    TigerClipboardServer clipboardServer;
    clipboardServer.initServer();
    clipboardServer.copy("a");
    std::cout << *clipboardServer.pasteIterator();
    clipboardServer.copy("b");
    clipboardServer.copy("c");
    clipboardServer.copy("d");
    clipboardServer.copy("e");

    std::deque<std::string> targetDeque ({"e"});

    EXPECT_EQ(clipboardServer.clipboard(), targetDeque);
}

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}