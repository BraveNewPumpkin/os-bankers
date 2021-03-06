//
// Created by Kyle Bolton on 10/10/16.
//

#ifndef TEST_INTERCOM_H
#define TEST_INTERCOM_H

#define BUFFER_SIZE 100

#include <string>
#include <memory>
#include <stdexcept>
#include <cstring>
#include <cerrno>
#include <unistd.h>

class InterCom {
private:
  bool is_parent;
  bool is_ready;
  int parent_to_child_pipe_in;
  int parent_to_child_pipe_out;
  int child_to_parent_pipe_in;
  int child_to_parent_pipe_out;

  void tell(const int& pipe_to_tell, const std::string &message);
  std::unique_ptr<std::string> listen(const int& pipe_to_listen);
public:
  InterCom();
  ~InterCom();
  void registerAsParent();
  void registerAsChild();
  void tellParent(const std::string& message);
  void tellChild(const std::string& message);
  std::unique_ptr<std::string> listenToParent();
  std::unique_ptr<std::string> listenToChild();
};


#endif //TEST_INTERCOM_H
