#include "gtest/gtest.h"
#include <iostream>
#include <sstream>
#include <list>

#include "../src/TigerClipboardServer.h"

std::list<std::string> executeOpString(std::string opString);
void printList(std::list<std::string> list);
void printDeque(std::deque<std::string> deque);

namespace {

void opStringRun(std::string opString, std::list<std::string> targetOrder);

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



TEST_F(TigerClipboardServerTest, OPSTRING_DEBUG) {
    std::string opString = "C a C b C c C d P P P";
    std::list<std::string> targetOrder {"d", "d", "d"};
    opStringRun(opString, targetOrder);
}

void opStringRun(std::string opString, std::list<std::string> targetOrder) {
    std::list<std::string> actualOrder = executeOpString(opString);
    
    std::cerr << "Target: " << "\t";
    printList(targetOrder);
    std::cerr << "Actual: " << "\t";
    printList(actualOrder);

    EXPECT_EQ(actualOrder, targetOrder);
}


}  // namespace

void printDeque(std::deque<std::string> deque) {
    int pos = 0;
    for (auto it = deque.begin(); it != deque.end(); ++it) {
        std::cerr << pos << "\t" << *it << "\n";
    }
}

void printList(std::list<std::string> list) {
    for (auto it = list.begin(); it != list.end(); ++it) {
        std::cerr << *it;
    }
    std::cerr << std::endl;
}

std::list<std::string> executeOpString(std::string opString) {
    TigerClipboardServer clipboardServer;
    clipboardServer.initServer();
    std::list<std::string> pasteOrder;
    std::istringstream iss(opString);

    std::string pasteBuff;
    
    std::string op;
    std::string copyString;

    std::string copyMode;
    std::string pasteMode;
    std::string offsetString;
    int itOffset;
    auto it = clipboardServer.clipboard().begin();
    while (iss >> op) {
        if (op =="C") {
            copyString;
            iss >> copyString;
            pasteBuff = clipboardServer.copy(copyString).second;
        } else if (op == "P") {
            pasteOrder.push_back(pasteBuff);
            printList(pasteOrder);
            pasteBuff = clipboardServer.paste().second;
        } else if (op == "SETCM") {
            iss >> copyMode;
            if (copyMode == "FRONT") clipboardServer.setCopyMode(TigerClipboardServer::CopyMode::COPY_FRONT);
            else if (copyMode == "BEFORE") clipboardServer.setCopyMode(TigerClipboardServer::CopyMode::COPY_BEFORE);
            else if (copyMode == "STATIC") clipboardServer.setCopyMode(TigerClipboardServer::CopyMode::COPY_STATIC);
            else if (copyMode == "AFTER") clipboardServer.setCopyMode(TigerClipboardServer::CopyMode::COPY_AFTER);
            else if (copyMode == "BACK") clipboardServer.setCopyMode(TigerClipboardServer::CopyMode::COPY_BACK);
            else {
                std::cerr << "ERROR: Invalid CopyMode: " << copyMode << std::endl;
                exit(1);
            }
        } else if (op == "SETPM") {
            iss >> pasteMode;
            if (pasteMode == "FRONT")
                pasteBuff = clipboardServer.setPasteMode(TigerClipboardServer::PasteMode::PASTE_FRONT);
            else if (pasteMode == "FORWARDS")
                pasteBuff = clipboardServer.setPasteMode(TigerClipboardServer::PasteMode::PASTE_FORWARDS);
            else if (pasteMode == "STATIC")
                pasteBuff = clipboardServer.setPasteMode(TigerClipboardServer::PasteMode::PASTE_STATIC);
            else if (pasteMode == "BACKWARDS")
                pasteBuff = clipboardServer.setPasteMode(TigerClipboardServer::PasteMode::PASTE_BACKWARDS);
            else if (pasteMode == "BACK")
                pasteBuff = clipboardServer.setPasteMode(TigerClipboardServer::PasteMode::PASTE_BACK);
            else {
                std::cerr << "ERROR: Invalid PasteMode: " << pasteMode << std::endl;
                exit(1);
            }
        } else if (op == "SETCI") {
            iss >> offsetString;
            try {
                itOffset = std::stoi(offsetString);
            } catch (std::invalid_argument& e) {
                std::cerr << "ERROR: Invalid iterator offset: " << offsetString << std::endl;
            }
            clipboardServer.setCopyIterator(itOffset);
        } else if (op == "SETPI") {
            iss >> offsetString;
            try {
            int itOffSet = std::stoi(offsetString);
            } catch (std::invalid_argument& e) {
                std::cerr << "ERROR: Invalid iterator offset: " << offsetString << std::endl;
            }
            pasteBuff = clipboardServer.setPasteIterator(itOffset).second;
        } else {
            std::cerr << "ERROR: Invalid op token: " << op << std::endl;
            exit(1);
        }
    }
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