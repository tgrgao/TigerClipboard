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

TEST_F(TigerClipboardServerTest, TestCopyStatic) {
    TigerClipboardServer clipboardServer;
    clipboardServer.initServer();
    clipboardServer.copy("a");
    clipboardServer.copy("b");
    clipboardServer.copy("c");
    clipboardServer.copy("d");
    clipboardServer.copy("e");

    std::deque<std::string> targetDeque ({"e"});

    EXPECT_EQ(clipboardServer.clipboard(), targetDeque);
}

TEST_F(TigerClipboardServerTest, TestCopyFront) {
    TigerClipboardServer clipboardServer;
    clipboardServer.initServer();
    clipboardServer.copy("a");
    clipboardServer.copy("b");
    clipboardServer.setCopyMode(TigerClipboardServer::CopyMode::COPY_FRONT);
    clipboardServer.copy("c");
    clipboardServer.copy("d");
    clipboardServer.copy("e");

    std::deque<std::string> targetDeque ({"e", "d", "c", "b"});

    EXPECT_EQ(clipboardServer.clipboard(), targetDeque);
}

TEST_F(TigerClipboardServerTest, TestCopyBack) {
    TigerClipboardServer clipboardServer;
    clipboardServer.initServer();
    clipboardServer.copy("a");
    clipboardServer.copy("b");
    clipboardServer.setCopyMode(TigerClipboardServer::CopyMode::COPY_BACK);
    clipboardServer.copy("c");
    clipboardServer.copy("d");
    clipboardServer.copy("e");

    std::deque<std::string> targetDeque ({"b", "c", "d", "e"});

    EXPECT_EQ(clipboardServer.clipboard(), targetDeque);
}

TEST_F(TigerClipboardServerTest, TestCopyBefore) {
    TigerClipboardServer clipboardServer;
    clipboardServer.initServer();
    clipboardServer.setCopyMode(TigerClipboardServer::CopyMode::COPY_BACK);
    clipboardServer.copy("a");
    clipboardServer.copy("b");
    clipboardServer.copy("c");
    clipboardServer.setCopyMode(TigerClipboardServer::CopyMode::COPY_BEFORE);
    clipboardServer.copy("d");
    clipboardServer.copy("e");

    std::deque<std::string> targetDeque ({"a", "b", "d", "e", "c"});

    EXPECT_EQ(clipboardServer.clipboard(), targetDeque);
}

TEST_F(TigerClipboardServerTest, TestCopyAfter) {
    TigerClipboardServer clipboardServer;
    clipboardServer.initServer();
    clipboardServer.setCopyMode(TigerClipboardServer::CopyMode::COPY_BACK);
    clipboardServer.copy("a");
    clipboardServer.copy("b");
    clipboardServer.copy("c");
    clipboardServer.setCopyMode(TigerClipboardServer::CopyMode::COPY_AFTER);
    clipboardServer.copy("d");
    clipboardServer.copy("e");

    std::deque<std::string> targetDeque ({"a", "b", "c", "e", "d"});

    EXPECT_EQ(clipboardServer.clipboard(), targetDeque);
}

TEST_F(TigerClipboardServerTest, DEBUG) {
    TigerClipboardServer clipboardServer;
    clipboardServer.initServer();
    clipboardServer.copy("a");
    clipboardServer.setCopyMode(TigerClipboardServer::CopyMode::COPY_FRONT);
    clipboardServer.copy("d");
    //clipboardServer.copy("e");

    std::deque<std::string> targetDeque ({"e", "d", "c"});

    //EXPECT_EQ(clipboardServer.clipboard(), targetDeque);
}


}  // namespace

void printDeque(std::deque<std::string> deque) {
    int pos = 0;
    for (auto iter = deque.begin(); iter != deque.end(); ++iter) {
        std::cout << pos << "\t" << *iter << "\n";
    }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}