//
// Created by Kyle Bolton on 10/10/16.
//

#include "InterCom.h"

InterCom::InterCom() {
  is_ready = false;
  int child_to_parent[2];
  int parent_to_child[2];
  if(pipe(child_to_parent) == -1){
    throw runtime_error("failed to open child_to_parent pipe: " + string(strerror(errno)));
  }
  if(pipe(parent_to_child) == -1){
    throw runtime_error("failed to open parent_to_child pipe: " + string(strerror(errno)));
  }
  child_to_parent_pipe_in = child_to_parent[0];
  child_to_parent_pipe_out = child_to_parent[1];
  parent_to_child_pipe_in = parent_to_child[0];
  parent_to_child_pipe_out = parent_to_child[1];
}

void InterCom::registerAsParent() {
  if(close(child_to_parent_pipe_in) == -1){
    throw runtime_error("failed to close child_to_parent_pipe_in: " + string(strerror(errno)));
  }
  if(close(parent_to_child_pipe_out) == -1){
    throw runtime_error("failed to close parent_to_child_pipe_out: " + string(strerror(errno)));
  }
  is_parent = true;
  is_ready = true;
}


void InterCom::registerAsChild() {
  if(close(child_to_parent_pipe_out) == -1){
    throw runtime_error("failed to close child_to_parent_pipe_out: " + string(strerror(errno)));
  }
  if(close(parent_to_child_pipe_in) == -1){
    throw runtime_error("failed to close parent_to_child_pipe_in: " + string(strerror(errno)));
  }
  is_parent = false;
  is_ready = true;
}

InterCom::~InterCom() {
  if(is_ready){
    if(is_parent){
      if(close(child_to_parent_pipe_out) == -1){
        throw runtime_error("failed to close child_to_parent_pipe_out: " + string(strerror(errno)));
      }
      if(close(parent_to_child_pipe_in) == -1){
        throw runtime_error("failed to close parent_to_child_pipe_in: " + string(strerror(errno)));
      }
    }else{
      if(close(child_to_parent_pipe_in) == -1){
        throw runtime_error("failed to close child_to_parent_pipe_in: " + string(strerror(errno)));
      }
      if(close(parent_to_child_pipe_out) == -1){
        throw runtime_error("failed to close parent_to_child_pipe_out: " + string(strerror(errno)));
      }
    }
  }else{
    if(close(child_to_parent_pipe_in) == -1){
      throw runtime_error("failed to close child_to_parent_pipe_in: " + string(strerror(errno)));
    }
    if(close(child_to_parent_pipe_out) == -1){
      throw runtime_error("failed to close child_to_parent_pipe_out: " + string(strerror(errno)));
    }
    if(close(parent_to_child_pipe_in) == -1){
      throw runtime_error("failed to close parent_to_child_pipe_in: " + string(strerror(errno)));
    }
    if(close(parent_to_child_pipe_out) == -1){
      throw runtime_error("failed to close parent_to_child_pipe_out: " + string(strerror(errno)));
    }
  }
}

void InterCom::tellChild(const string &message) {
  if(!is_parent){
    throw runtime_error("attempting to tell child as child");
  }
  tell(parent_to_child_pipe_in, message);
}


void InterCom::tellParent(const string &message) {
  if(is_parent){
    throw runtime_error("attempting to tell parent as parent");
  }
  tell(child_to_parent_pipe_in, message);
}

void InterCom::tell(const int& pipe_to_tell, const string &message){
  if(message.size() > BUFFER_SIZE){
    throw runtime_error("attempting to write " + to_string(message.size()) + " bytes to " + to_string(BUFFER_SIZE) + " byte buffer");
  }
  string buffered_string = message;
  if(message.size() < BUFFER_SIZE){
    //pad to BUFFER_SIZE on right with \0s (null characters)
    buffered_string.insert(buffered_string.end(), BUFFER_SIZE - message.size(), '\0');
  }
  int result = write(pipe_to_tell, buffered_string.c_str(), BUFFER_SIZE);
  if(result != BUFFER_SIZE){
    throw runtime_error("failed to write full string to pipe: " + string(strerror(errno)));
  }
}

unique_ptr<string> InterCom::listenToChild() {
  if(!is_parent){
    throw runtime_error("attempting to listen to child as child");
  }
  return listen(child_to_parent_pipe_out);
}

unique_ptr<string> InterCom::listenToParent() {
  if(is_parent){
    throw runtime_error("attempting to listen to parent as parent");
  }
  return listen(parent_to_child_pipe_out);
}

unique_ptr<string> InterCom::listen(const int &pipe_to_listen) {
  char buffer[BUFFER_SIZE];
  ssize_t result = read(pipe_to_listen, buffer, BUFFER_SIZE);
  if(result != BUFFER_SIZE){
    throw runtime_error("failed to read full buffer from pipe: " + string(strerror(errno)));
  }
  return make_unique<string>(buffer);
}
