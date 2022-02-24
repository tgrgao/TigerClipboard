#include "gtest/gtest.h"
#include <iostream>
#include <sstream>

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

TEST_F(TigerClipboardServerTest, TestPasteStatic) {
    TigerClipboardServer clipboardServer;
    clipboardServer.initServer();
    clipboardServer.setCopyMode(TigerClipboardServer::CopyMode::COPY_BACK);
    clipboardServer.copy("a");
    clipboardServer.copy("b");
    clipboardServer.copy("c");
    clipboardServer.copy("d");
    clipboardServer.copy("e");

    std::string nextUp = clipboardServer.setPasteMode(TigerClipboardServer::PasteMode::PASTE_STATIC);
    EXPECT_EQ(nextUp, "a");

    nextUp = clipboardServer.paste().second;
    std::deque<std::string> targetDeque ({"a", "b", "c", "d", "e"});

    EXPECT_EQ(clipboardServer.clipboard(), targetDeque);
    EXPECT_EQ(nextUp, "a");

    nextUp = clipboardServer.paste().second;

    EXPECT_EQ(clipboardServer.clipboard(), targetDeque);
    EXPECT_EQ(nextUp, "a");
    
}

TEST_F(TigerClipboardServerTest, TestPasteFront) {
    TigerClipboardServer clipboardServer;
    clipboardServer.initServer();
    clipboardServer.setCopyMode(TigerClipboardServer::CopyMode::COPY_BACK);
    clipboardServer.copy("a");
    clipboardServer.copy("b");
    clipboardServer.copy("c");
    clipboardServer.copy("d");
    clipboardServer.copy("e");

    std::string nextUp = clipboardServer.setPasteMode(TigerClipboardServer::PasteMode::PASTE_FRONT);
    EXPECT_EQ(nextUp, "a");

    nextUp = clipboardServer.paste().second;
    std::deque<std::string> targetDeque ({"b", "c", "d", "e"});

    EXPECT_EQ(clipboardServer.clipboard(), targetDeque);
    EXPECT_EQ(nextUp, "b");

    nextUp = clipboardServer.paste().second;

    targetDeque.assign({"c", "d", "e"});
    EXPECT_EQ(clipboardServer.clipboard(), targetDeque);
    EXPECT_EQ(nextUp, "c");
}

TEST_F(TigerClipboardServerTest, TestPasteBack) {
    TigerClipboardServer clipboardServer;
    clipboardServer.initServer();
    clipboardServer.setCopyMode(TigerClipboardServer::CopyMode::COPY_BACK);
    clipboardServer.copy("a");
    clipboardServer.copy("b");
    clipboardServer.copy("c");
    clipboardServer.copy("d");
    clipboardServer.copy("e");

    std::string nextUp = clipboardServer.setPasteMode(TigerClipboardServer::PasteMode::PASTE_BACK);
    EXPECT_EQ(nextUp, "e");

    nextUp = clipboardServer.paste().second;
    std::deque<std::string> targetDeque ({"a", "b", "c", "d"});

    EXPECT_EQ(clipboardServer.clipboard(), targetDeque);
    EXPECT_EQ(nextUp, "d");

    nextUp = clipboardServer.paste().second;

    targetDeque.assign({"a", "b", "c"});
    EXPECT_EQ(clipboardServer.clipboard(), targetDeque);
    EXPECT_EQ(nextUp, "c");
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
        std::cerr << pos << "\t" << *iter << "\n";
    }
}

std::list<std::string> executeOpSet(std::string opSet) {
    TigerClipboardServer clipboardServer;
    clipboardServer.initServer();
    std::list<std::string> pasteOrder;
    std::istringstream iss(opSet);
    std::string pasteBuff;
    do {
        std::string op;
        iss >> op;
        switch(op) {
            case "C":
                std::string copyString;
                iss >> copyString;
                pasteBuff = clipboardServer.copy(copyString);
                break;
            case "P":
                pasteOrder.push_back(pasteBuff);
                pasteBuff = clipboardServer.paste();
                break;
            case "SETCM":
                std::string mode;
                iss >> mode;
                switch(mode) {
                    case "FRONT":
                        clipboardServer.setCopyMode(TigerClipboardServer::CopyMode::COPY_FRONT);
                        break;
                    case "BEFORE":
                        clipboardServer.setCopyMode(TigerClipboardServer::CopyMode::COPY_BEFORE);
                        break;
                    case "STATIC":
                        clipboardServer.setCopyMode(TigerClipboardServer::CopyMode::COPY_STATIC);
                        break;
                    case "AFTER":
                        clipboardServer.setCopyMode(TigerClipboardServer::CopyMode::COPY_AFTER);
                        break;
                    case "BACK":
                        clipboardServer.setCopyMode(TigerClipboardServer::CopyMode::COPY_BACK);
                        break;
                    default:
                        std::cerr << "ERROR: Invalid CopyMode: " << mode << std::endl;
                        exit(1);
                }
                break;
            case "SETPM":
                std::string mode;
                iss >> mode;
                switch(mode) {
                    case "FRONT":
                        clipboardServer.setPasteMode(TigerClipboardServer::PasteMode::PASTE_FRONT);
                        break;
                    case "FORWARDS":
                        clipboardServer.setPasteMode(TigerClipboardServer::PasteMode::PASTE_FORWARDS);
                        break;
                    case "STATIC":
                        clipboardServer.setPasteMode(TigerClipboardServer::PasteMode::PASTE_STATIC);
                        break;
                    case "BACKWARDS":
                        clipboardServer.setPasteMode(TigerClipboardServer::PasteMode::PASTE_BACKWARDS);
                        break;
                    case "BACK":
                        clipboardServer.setPasteMode(TigerClipboardServer::PasteMode::PASTE_BACK);
                        break;
                    default:
                        std::cerr << "ERROR: Invalid PasteMode: " << mode << std::endl;
                        exit(1);
                }
                break;
            case "SETCI":
                std::string offSetString;
                iss >> offSetString;
                try {
                int itOffSet = std::stoi(offSetString);
                } catch (std::invalid_argument& e) {
                    std::cerr << "ERROR: Invalid iterator offset: " << offSetString << std::endl;
                }
                auto it = clipboardServer.clipboard().begin();
                for (int i = 0; i < itOffSet; ++i) ++it;
                clipboardServer.setCopyIterator(it);
                break;
            case "SETPI":
                std::string offSetString;
                iss >> offSetString;
                try {
                int itOffSet = std::stoi(offSetString);
                } catch (std::invalid_argument& e) {
                    std::cerr << "ERROR: Invalid iterator offset: " << offSetString << std::endl;
                }
                auto it = clipboardServer.clipboard().begin();
                for (int i = 0; i < itOffSet; ++i) ++it;
                pasteBuff = clipboardServer.setPasteIterator(it);
                break;
            default:
                std::cerr << "ERROR: Invalid op token: " << op << std::endl;
        }
    } while (iss);
    return pasteOrder;
}

/*
void comparePasteOrder(TigerClipboardServer clipboardServer, std::list<std::string> targetOrder) {
    std::list<std::string> pasteOrder;
    std::string upNext = clipboardServer.upNext();
    while(upNext != "") {
        targetOrder.push_back(upNext);
        upNext = clipboardServer.paste();
    }
}
*/

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}