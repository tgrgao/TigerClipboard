#include <string>
#include <deque>
#include <utility>

class TigerClipboardServer {
    public:
        enum Status {OK, NOT_OK};
        enum CopyMode {FRONT, BEFORE, STATIC, AFTER, BACK};
        enum PasteMode {FRONT, FORWARDS, STATIC, BACKWARDS, BACK};

        Status initServer();
        CopyMode setCopyMode(CopyMode newCopyMode);
        PasteMode setPasteMode(PasteMode newPasteMode);
        std::pair<Status, std::string> copy(std::string copiedString);
        std::pair<Status, std::string> paste();

        const Status getStatus() const {return status_;}
        const CopyMode copyMode() const {return copyMode_;}
        const PasteMode pasteMode() const {return pasteMode_;}
        const std::deque<std::string>& getClipboard() const {return clipboard_;}
        const std::deque<std::string>::iterator& getCopyIterator() const {return copyIterator_;};
        const std::deque<std::string>::iterator& getPasteIterator() const {return pasteIterator_;}

    private:
        Status status_;
        CopyMode copyMode_;
        PasteMode pasteMode_;
        std::deque<std::string> clipboard_;
        std::deque<std::string>::iterator copyIterator_;
        std::deque<std::string>::iterator pasteIterator_;
};