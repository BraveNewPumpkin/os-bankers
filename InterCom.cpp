//
// Created by Kyle Bolton on 10/10/16.
//

#include "InterCom.h"

InterCom::InterCom() {
  is_ready = false;
  is_parent = true;
  int child_to_parent[2];
  int parent_to_child[2];
  if(pipe(child_to_parent) == -1){
    throw std::runtime_error("failed to open child_to_parent pipe: " + std::string(strerror(errno)));
  }
  if(pipe(parent_to_child) == -1){
    throw std::runtime_error("failed to open parent_to_child pipe: " + std::string(strerror(errno)));
  }

  child_to_parent_pipe_in = child_to_parent[1];
  child_to_parent_pipe_out = child_to_parent[0];
  parent_to_child_pipe_in = parent_to_child[1];
  parent_to_child_pipe_out = parent_to_child[0];
}

void InterCom::registerAsParent() {
  is_parent = true;
  if(close(child_to_parent_pipe_in) == -1){
    throw std::runtime_error("failed to close child_to_parent_pipe_in: " + std::string(strerror(errno)));
  }
  if(close(parent_to_child_pipe_out) == -1){
    throw std::runtime_error("failed to close parent_to_child_pipe_out: " + std::string(strerror(errno)));
  }
  is_ready = true;
}


void InterCom::registerAsChild() {
  is_parent = false;
  if(close(child_to_parent_pipe_out) == -1){
    throw std::runtime_error("failed to close child_to_parent_pipe_out: " + std::string(strerror(errno)));
  }
  if(close(parent_to_child_pipe_in) == -1){
    throw std::runtime_error("failed to close parent_to_child_pipe_in: " + std::string(strerror(errno)));
  }
  is_ready = true;
}

InterCom::~InterCom() {
  if(is_ready){
    if(is_parent){
      if(close(child_to_parent_pipe_out) == -1){
        throw std::runtime_error("failed to close child_to_parent_pipe_out: " + std::string(strerror(errno)));
      }
      child_to_parent_pipe_out = -2;
      if(close(parent_to_child_pipe_in) == -1){
        throw std::runtime_error("failed to close parent_to_child_pipe_in: " + std::string(strerror(errno)));
      }
      parent_to_child_pipe_in = -2;
    }else{
      if(close(child_to_parent_pipe_in) == -1){
        throw std::runtime_error("failed to close child_to_parent_pipe_in: " + std::string(strerror(errno)));
      }
      child_to_parent_pipe_in = -2;
      if(close(parent_to_child_pipe_out) == -1){
        throw std::runtime_error("failed to close parent_to_child_pipe_out: " + std::string(strerror(errno)));
      }
      parent_to_child_pipe_out = -2;
    }
  }else{
    if(close(child_to_parent_pipe_in) == -1){
      throw std::runtime_error("failed to close child_to_parent_pipe_in: " + std::string(strerror(errno)));
    }
    child_to_parent_pipe_in = -3;
    if(close(child_to_parent_pipe_out) == -1){
      throw std::runtime_error("failed to close child_to_parent_pipe_out: " + std::string(strerror(errno)));
    }
    child_to_parent_pipe_out = -3;
    if(close(parent_to_child_pipe_in) == -1){
      throw std::runtime_error("failed to close parent_to_child_pipe_in: " + std::string(strerror(errno)));
    }
    parent_to_child_pipe_in = -3;
    if(close(parent_to_child_pipe_out) == -1){
      throw std::runtime_error("failed to close parent_to_child_pipe_out: " + std::string(strerror(errno)));
    }
    parent_to_child_pipe_out = -3;
  }
}

void InterCom::tellChild(const std::string &message) {
  if(!is_parent){
    throw std::runtime_error("attempting to tell child as child");
  }
  tell(parent_to_child_pipe_in, message);
}


void InterCom::tellParent(const std::string &message) {
  if(is_parent){
    throw std::runtime_error("attempting to tell parent as parent");
  }
  tell(child_to_parent_pipe_in, message);
}

void InterCom::tell(const int& pipe_to_tell, const std::string &message){
  if(message.size() > BUFFER_SIZE){
    throw std::runtime_error("attempting to write " + std::to_string(message.size()) + " bytes to " + std::to_string(BUFFER_SIZE) + " byte buffer");
  }
  ssize_t result;
  do {
    result = write(pipe_to_tell, message.c_str(), BUFFER_SIZE);
    //ignore EINTR errors as they are apparently no big deal and indicate we should retry
    if (result == -1 && errno != EINTR) {
      throw std::runtime_error("failed to write string to pipe: " + std::string(strerror(errno)));
    }
  } while(result == -1);
}

std::unique_ptr<std::string> InterCom::listenToChild() {
  if(!is_parent){
    throw std::runtime_error("attempting to listen to child as child");
  }
  return listen(child_to_parent_pipe_out);
}

std::unique_ptr<std::string> InterCom::listenToParent() {
  if(is_parent){
    throw std::runtime_error("attempting to listen to parent as parent"); }
  return listen(parent_to_child_pipe_out);
}

std::unique_ptr<std::string> InterCom::listen(const int &pipe_to_listen) {
  char buffer[BUFFER_SIZE];
  ssize_t result;
  do{
    result = read(pipe_to_listen, buffer, BUFFER_SIZE);
    //ignore EINTR errors as they are apparently no big deal and indicate we should retry
    if (result == -1 && errno != EINTR){
      throw std::runtime_error("failed to read buffer from pipe: " + std::string(strerror(errno)));
    }
  }while(result == -1);
  return std::make_unique<std::string>(buffer);
}
