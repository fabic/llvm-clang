#include <cassert>
#include "TextBuffers.hpp"
#include "logging.hpp"

namespace sf {

  TextBuffer::pointer
    TextBuffers::openFile(const filename_t filename)
  {
    //assert(filename_ != nullptr && "Null filename string pointer.");
    //assert(filename_[0] != '\0' && "Empty filename string.");

    std::pair<TextBuffer_map_t::iterator, bool> insertionStatus =
        _buffers.emplace(
            std::make_pair(
                filename,
                std::make_shared< TextBuffer >(filename)
            )
        );

    auto& inserted = insertionStatus.second;

    if (! inserted) {
      throw sf::ex::yet_undefined_exception(
          "Failed to emplace() a new TextBuffer into our map<>.");
    }

    TextBuffer::pointer tb_ = insertionStatus.first->second;

    // Read file content from here now.
    logtrace << "TextBuffers::openFile(" << filename << "): "
                "We're reading the whole file content now (WIP/IMPL.)" ;

    tb_->openFile();

    return tb_;
  }
}