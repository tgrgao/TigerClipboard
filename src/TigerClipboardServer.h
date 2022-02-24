#include <string>
#include <deque>
#include <utility>

class TigerClipboardServer {
    public:
        enum Status {OK, NOT_OK};
        enum CopyMode {COPY_FRONT, COPY_BEFORE, COPY_STATIC, COPY_AFTER, COPY_BACK};
        enum PasteMode {PASTE_FRONT, PASTE_FORWARDS, PASTE_STATIC, PASTE_BACKWARDS, PASTE_BACK};

        Status initServer();
        std::string setCopyMode(CopyMode newCopyMode);
        std::string setPasteMode(PasteMode newPasteMode);
        std::pair<Status, std::string> setCopyIterator(const std::deque<std::string>::iterator& newCopyIterator);
        std::pair<Status, std::string> setPasteIterator(const std::deque<std::string>::iterator& newPasteIterator);
        std::pair<Status, std::string> copy(std::string copiedString);
        std::pair<Status, std::string> paste();
        std::string upNext() {return *pasteIterator_;}

        const Status status() const {return status_;}
        const CopyMode copyMode() const {return copyMode_;}
        const PasteMode pasteMode() const {return pasteMode_;}
        const std::deque<std::string>& clipboard() const {return clipboard_;}
        const std::deque<std::string>::iterator& copyIterator() const {return copyIterator_;}
        const std::deque<std::string>::iterator& pasteIterator() const {return pasteIterator_;}

    private:
        Status status_;
        CopyMode copyMode_;
        PasteMode pasteMode_;
        std::deque<std::string> clipboard_;
        std::deque<std::string>::iterator copyIterator_;
        std::deque<std::string>::iterator pasteIterator_;
        void fixIteratorsFrontBack();
};