#include "TigerClipboardServer.h"
#include <iostream>

TigerClipboardServer::Status TigerClipboardServer::initServer() {
    status_ = Status::OK;
    copyMode_ = CopyMode::COPY_STATIC;
    pasteMode_ = PasteMode::PASTE_STATIC;
    copyIterator_ = clipboard_.begin();
    pasteIterator_ = copyIterator_;
    
    if (pasteIterator_ != clipboard_.end()) return Status::NOT_OK; //sanity check
    return Status::OK;
}

void TigerClipboardServer::fixIteratorsFrontBack() {
    if (clipboard_.size() < 2) {
        copyIterator_ = clipboard_.begin();
        pasteIterator_ = copyIterator_;
        return;
    }

    switch(copyMode_) {
        case CopyMode::COPY_FRONT:
            copyIterator_ = clipboard_.begin();
            break;
        case CopyMode::COPY_BACK:
            copyIterator_ = clipboard_.end() - 1;
            break;
        default:
            if (copyIterator_ == clipboard_.end() && copyIterator_ != clipboard_.begin()) --copyIterator_;
            break;
    }
    
    switch(pasteMode_) {
        case PasteMode::PASTE_FRONT:
            pasteIterator_ = clipboard_.begin();
            break;
        case PasteMode::PASTE_BACK:
            pasteIterator_ = clipboard_.end() - 1;
            break;
        default:
            if (pasteIterator_ == clipboard_.end() && pasteIterator_ != clipboard_.begin()) --pasteIterator_;
            break;
    }
}

std::string TigerClipboardServer::setCopyMode(CopyMode newCopyMode) {
    copyMode_ = newCopyMode;
    fixIteratorsFrontBack();
    if (!clipboard_.empty()) return *pasteIterator_;
    return "";
}

std::string TigerClipboardServer::setPasteMode(PasteMode newPasteMode) {
    pasteMode_ = newPasteMode;
    fixIteratorsFrontBack();
    if (!clipboard_.empty()) return *pasteIterator_;
    return "";
}

std::pair<TigerClipboardServer::Status, std::string> TigerClipboardServer::setCopyIterator(const std::deque<std::string>::iterator& newCopyIterator) {
    copyIterator_ = clipboard_.erase(newCopyIterator, newCopyIterator);
    return std::make_pair(Status::OK, *pasteIterator_);
}

std::pair<TigerClipboardServer::Status, std::string> TigerClipboardServer::setPasteIterator(const std::deque<std::string>::iterator& newPasteIterator) {
    pasteIterator_ = clipboard_.erase(newPasteIterator, newPasteIterator);
    return std::make_pair(Status::OK, *pasteIterator_);
}

std::pair<TigerClipboardServer::Status, std::string> TigerClipboardServer::copy(std::string copiedString) {
    if (clipboard_.size() == 0) {
        clipboard_.push_back(copiedString);
        copyIterator_ = clipboard_.begin();
        pasteIterator_ = copyIterator_;
        return std::make_pair(Status::OK, copiedString);
    }
    
    int pasteItOffset = 0;
    if (copyMode_ != CopyMode::COPY_STATIC) {
    for (auto it = clipboard_.begin(); it != pasteIterator_; ++it) ++pasteItOffset;
    }

    switch(copyMode_) {
        case CopyMode::COPY_FRONT:
            ++pasteItOffset;
            clipboard_.push_front(copiedString);
            copyIterator_ = clipboard_.begin();
            pasteIterator_ = clipboard_.begin() + pasteItOffset;
            break;
        case CopyMode::COPY_BEFORE:
            if (copyIterator_ <= pasteIterator_) ++pasteItOffset;
            copyIterator_ = clipboard_.insert(copyIterator_, copiedString) + 1;
            pasteIterator_ = clipboard_.begin() + pasteItOffset;
            break;
        case CopyMode::COPY_STATIC:
            *copyIterator_ = copiedString;
            break;
        case CopyMode::COPY_AFTER:
            if (copyIterator_ < pasteIterator_) ++pasteItOffset;
            copyIterator_ = clipboard_.insert(++copyIterator_, copiedString) - 1;
            pasteIterator_ = clipboard_.begin() + pasteItOffset;
            break;
        case CopyMode::COPY_BACK:
            clipboard_.push_back(copiedString);
            copyIterator_ = clipboard_.end() - 1;
            pasteIterator_ = clipboard_.begin() + pasteItOffset;
            break;
        default:
            return std::make_pair(Status::NOT_OK, ""); //should be unreachable
    }
    fixIteratorsFrontBack();
    return std::make_pair(Status::OK, *pasteIterator_);
}

std::pair<TigerClipboardServer::Status, std::string> TigerClipboardServer::paste() {
    if (clipboard_.size() == 0) return std::make_pair(Status::OK, "");
    bool needToReassignCopyIterator = false;
    switch(pasteMode_) {
        case PasteMode::PASTE_FRONT:
            if (copyIterator_ == clipboard_.begin()) ++copyIterator_;
            clipboard_.pop_front();
            pasteIterator_ = clipboard_.begin();
            break;
        case PasteMode::PASTE_FORWARDS:
            if (copyIterator_ == pasteIterator_) needToReassignCopyIterator = true;
            if (pasteIterator_ + 1 == clipboard_.end()) { //if pasteIterator is at last element, erase and decrement instead, and if copyIterator needs to be reassigned it will just be reassigned to new pasteIterator regardless of copyMode
                clipboard_.erase(pasteIterator_--);
                if (needToReassignCopyIterator) copyIterator_ = pasteIterator_;
                break;
            }

            clipboard_.erase(pasteIterator_++);
            if (needToReassignCopyIterator) {
                switch(copyMode_) {
                    case CopyMode::COPY_FRONT:
                        copyIterator_ = clipboard_.begin();
                        break;
                    case CopyMode::COPY_BEFORE:
                        copyIterator_ = pasteIterator_; //we want to insert before the destroyed item the old copyIterator was at, we want the new copyIterator to point to one incremented from the deleted item, since the pasteIterator was already incremented from where the old copyIterator was at, we can leave as is
                        break;
                    case CopyMode::COPY_STATIC:
                        copyIterator_ = pasteIterator_;
                        break;
                    case CopyMode::COPY_AFTER:
                        if (pasteIterator_ != clipboard_.begin()) copyIterator_ = --pasteIterator_; //we want to check to see that we're not at the beginning of the deque, if so we decrement to get the iterator to one item before the deleted item to get desired insert after behavior
                        else copyIterator_ = pasteIterator_; //else we just take the current pasteIterator
                        break;
                    case CopyMode::COPY_BACK:
                        copyIterator_ = clipboard_.end();
                        break;
                    default:
                        break;
                }
            }
            break;
        case PasteMode::PASTE_STATIC:
            break;
        case PasteMode::PASTE_BACKWARDS:
            if (copyIterator_ == pasteIterator_) needToReassignCopyIterator = true;
            if (pasteIterator_  == clipboard_.begin()) { //if pasteIterator is at first element, erase and increment instead, and if copyIterator needs to be reassigned it will just be reassigned to new pasteIterator regardless of copyMode
                clipboard_.erase(pasteIterator_++);
                if (needToReassignCopyIterator) copyIterator_ = pasteIterator_;
                break;
            }

            clipboard_.erase(pasteIterator_--);
            if (needToReassignCopyIterator) {
                switch(copyMode_) {
                    case CopyMode::COPY_FRONT:
                        copyIterator_ = clipboard_.begin();
                        break;
                    case CopyMode::COPY_BEFORE:
                        if (pasteIterator_ + 1 != clipboard_.end()) copyIterator_ = ++pasteIterator_; //we want to check to see that we're not at the end of the deque, if so we increment to get the iterator to one item after the deleted item to get desired insert before behavior
                        else copyIterator_ = pasteIterator_; //else we just take the current pasteIterator
                        break;
                    case CopyMode::COPY_STATIC:
                        copyIterator_ = pasteIterator_;
                    case CopyMode::COPY_AFTER:
                        copyIterator_ = pasteIterator_; //we want to insert after the destroyed item the old copyIterator was at, we want the new copyIterator to point to one decremented from the deleted item, since the pasteIterator was already decremented from where the old copyIterator was at, we can leave as is
                        break;
                    case CopyMode::COPY_BACK:
                        copyIterator_ = clipboard_.end();
                        break;
                    default:
                        break;
                }
            }
            break;
        case PasteMode::PASTE_BACK:
            if (copyIterator_ + 1 == clipboard_.end()) --copyIterator_;
            clipboard_.pop_back();
            pasteIterator_ = clipboard_.end() - 1;
            break;
        default:
            return std::make_pair(Status::NOT_OK, ""); //should be unreachable
    }
    fixIteratorsFrontBack();
    if (clipboard_.size() == 0) return std::make_pair(Status::OK, "");
    return std::make_pair(Status::OK, *pasteIterator_);
}