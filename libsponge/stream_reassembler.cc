#include "stream_reassembler.hh"
// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    if(eof){
        _eof_index = index + data.size();
        _eof_flag = true;
    }
    size_t len = data.length();

    if(index > _wait_index){
        if(full() || _output.input_ended()){
            return;
        }

        if(_unreassemble.count(index) != 0){
            _unreassemble[index] = data;
        }else{
            _unreassemble.insert({index, data});
        }
        
        merge_unassembled_bytes();
    }else{
        if((index + len) >= _wait_index){
            size_t num_needed_write = index + len  -_wait_index;
            size_t write_len = _output.write(string().assign(data.begin() + (_wait_index - index), data.end()));
            _wait_index += write_len;
            if(write_len < num_needed_write){
                _unreassemble.insert({_wait_index, string().assign(data.begin() + (_wait_index - index), data.end())});
                merge_unassembled_bytes();
            }
        }
    }

    push_unassembled_bytes();

    if(_eof_flag && _wait_index == _eof_index){
        _output.end_input();
    }

    calculate_num_unassemble_bytes();
} 

size_t StreamReassembler::unassembled_bytes() const { return _num_unassemble_bytes; }

bool StreamReassembler::empty() const { return _unreassemble.size() == 0; }

void StreamReassembler::merge_unassembled_bytes(){
    std::map<size_t, std::string>::iterator it = _unreassemble.begin();
    size_t f_index = it->first;
    std::string f_string = it->second;
    size_t f_end = f_index + f_string.length();
    std::map<size_t, std::string>::iterator f_it = it;
    size_t cur_index{};
    std::string cur_string{};
    size_t cur_end{};
    ++it;
    while (it != _unreassemble.cend())
    {
        cur_index = it->first;
        cur_string = it->second;
        cur_end = cur_index + cur_string.length();
        if(cur_index <= f_end) {
            if(cur_end > f_end){
                f_it->second = f_string + string().assign(cur_string.begin() + (f_end - cur_index), cur_string.end());
                f_end = cur_end;
                f_string = f_it->second;
            }
            it = _unreassemble.erase(it);
        }
        else{
            f_index = it->first;
            f_string = it->second;
            f_end = f_string.size() + f_index;
            ++it;
        }
    }
 
}

void StreamReassembler::calculate_num_unassemble_bytes(){
    std::map<size_t, std::string>::iterator it = _unreassemble.begin();
    size_t new_num = 0;
    while (it != _unreassemble.cend())
    {
        new_num += it->second.length();
        it++;
    }
    
    _num_unassemble_bytes = new_num;
}

bool StreamReassembler::full(){
    push_unassembled_bytes();
    return unassembled_bytes() == _capacity;
}

void StreamReassembler::push_unassembled_bytes(){
    if(_unreassemble.size() == 0){
        return;
    }
    std::map<size_t, std::string>::iterator it = _unreassemble.begin();
    while (it != _unreassemble.end() && ((it->first + it->second.length() - 1) < _wait_index))
    {
        it = _unreassemble.erase(it);
    }
    
    if(it != _unreassemble.end() && it->first <= _wait_index && (it->first + it->second.length() - 1) >= _wait_index){
        size_t write_len = _output.write(string().assign(it->second.begin() + (_wait_index - it->first), it->second.end()));
        _wait_index += write_len;
        if(write_len < it->second.length()){
            int num_unwrite = it->second.length() - write_len;
            it->second = string().assign(it->second.end() - num_unwrite, it->second.end());            
        }else{
            _unreassemble.erase(it);
        }
    }
}
