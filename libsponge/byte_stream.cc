#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) : _capacity(capacity) { }

size_t ByteStream::write(const string &data) {
    if(input_ended() || (remaining_capacity()) == 0){
        return 0;
    }
    size_t len = data.size();
    if(len > remaining_capacity()){
        len = remaining_capacity();
    }
    for(size_t i = 0; i < len; i++){
        _bytes.push_back(data[i]);
    }

    _bytes_written += len;

    return len;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    size_t new_len = len;
    if(new_len > _bytes.size()){
        new_len = _bytes.size();
    }
    std::string res{};
    for(size_t i = 0; i < new_len; i++){
        res += _bytes[i];
    }
    return res;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) { 
    size_t new_len = len;
    if(new_len > _bytes.size()){
        new_len = _bytes.size();
    }
    for(size_t i = 0; i < new_len; i++){
        _bytes.pop_front();
    }
    _bytes_read += new_len;
 }

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    if(eof()){
        return {};
    }
    string res = peek_output(len);
    pop_output(res.length());
    return res;
}

void ByteStream::end_input() {
    _end_input = true;
}

bool ByteStream::input_ended() const { return _end_input; }

size_t ByteStream::buffer_size() const { return _bytes.size(); }

bool ByteStream::buffer_empty() const { return _bytes.size() == 0; }

bool ByteStream::eof() const { return buffer_empty() && input_ended(); }

size_t ByteStream::bytes_written() const { return _bytes_written; }

size_t ByteStream::bytes_read() const { return _bytes_read; }

size_t ByteStream::remaining_capacity() const { return _capacity - _bytes.size(); }