#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

using namespace std;

ByteStream::ByteStream(const size_t capacity) : _capacity(capacity) {  }

size_t ByteStream::write(const string &data) {
    int data_len = data.length();
    int reamain_capacity = _capacity - _buffer.size();
    int write_len = data_len > reamain_capacity ? reamain_capacity : data_len;
    _write_count += write_len;
    int i = 0;
    while (i < write_len)
    {
        _buffer.push_back(data[i]);
        i++;
    }
    
    return write_len; 
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    int peek_len = len > _buffer.size() ? _buffer.size() : len;
    return string().assign(_buffer.begin(), _buffer.begin() + peek_len);
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) { 
    int pop_len = len > _buffer.size() ? _buffer.size() : len;
    _read_count += pop_len;
    while(pop_len > 0){
        _buffer.pop_front();
        pop_len--;
    }
 }

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    int read_len = len > _buffer.size() ? _buffer.size() : len;
    int i = 0; 
    string res;
    _read_count += read_len;
    while (i < read_len)
    {
        res += _buffer[i];
        _buffer.pop_front();
        i++;
    }
    
    return res;
}

void ByteStream::end_input() { _input_end = true; }

bool ByteStream::input_ended() const { return _input_end; }

size_t ByteStream::buffer_size() const { return _buffer.size(); }

bool ByteStream::buffer_empty() const { return _buffer.size() == 0; }

bool ByteStream::eof() const { return input_ended() && buffer_empty(); }

size_t ByteStream::bytes_written() const { return _write_count; }

size_t ByteStream::bytes_read() const { return _read_count; }

size_t ByteStream::remaining_capacity() const { return _capacity - _buffer.size(); }
