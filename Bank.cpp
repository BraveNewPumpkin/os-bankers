//
// Created by Kyle Bolton on 10/6/16.
//

#include "Bank.h"

const unsigned int Bank::addResource(const unsigned int& num_instances) {
  resources.push_back(std::make_unique<Resource>(resources.size() - 1, num_instances));
  return resources.size() - 1;
}

std::unique_ptr<Resource>& Bank::getResource(const int& resource_id) {
  return resources[resource_id];
}

bool Bank::releaseResources(const std::unique_ptr<Process>& process, std::vector<unsigned int> released_resources) {
  std::vector<unsigned int>::iterator released_resources_iter;
  std::vector<std::unique_ptr<Resource> >::iterator resources_iter;
  for(
     released_resources_iter = released_resources.begin(), resources_iter = resources.begin();
     released_resources_iter != released_resources.end() && resources_iter != resources.end();
     ++released_resources_iter, ++resources_iter
     ){
    const unsigned int process_index = process->getIndex();
    if(debits->at(process_index).at((*resources_iter)->getIndex()) >= *released_resources_iter){
      (*resources_iter)->release(*released_resources_iter);
    }else{
      throw std::runtime_error("attempting to release more resources than allocated for process_index: " + std::to_string(process_index));
    }
  }
  return true;
}

bool Bank::requestApproval(const std::unique_ptr<Process>& process, const Process::Instruction& instruction, const std::vector<unsigned int>& args){
  bool approval;
  switch(instruction){
    case Process::Instruction::calculate:
    case Process::Instruction::useresources:
      approval = true;
      break;
    case Process::Instruction::release:
      approval = releaseResources(process, args);
      break;
    case Process::Instruction::request:
      approval = bankers(process, args);
      break;
  }

    return approval;
}

bool Bank::bankers(const std::unique_ptr<Process>& process, std::vector<unsigned int> requested_resources) {
  //check requested_resources <= claim - debits for this process
  //check requested_resources >= available
  std::vector<unsigned int>::iterator requested_resources_iter;
  std::vector<std::unique_ptr<Resource> >::iterator resources_iter;
  for(
     requested_resources_iter = requested_resources.begin(), resources_iter = resources.begin();
     requested_resources_iter != requested_resources.end() && resources_iter != resources.end();
     ++requested_resources_iter, ++resources_iter
     ) {
    const unsigned int resource_index = (*resources_iter)->getIndex();
    const unsigned int process_index = process->getIndex();
    const unsigned int requested_resource_amount = *requested_resources_iter;
    const unsigned int resource_available_amount = (*resources_iter)->getAvailable();
    int need = claims->at(process_index).at(resource_index) - debits->at(process_index).at(resource_index);

    if (need < requested_resource_amount) {
      std::ostringstream error_sstream;
      error_sstream << "process (" << std::string(*process) << ") requested more than declared maximum of resource (";
      error_sstream << std::string(**resources_iter) << ")";
      throw std::runtime_error(error_sstream.str());
    }
    if(requested_resource_amount > resource_available_amount){
      return false;
    }
//      (*resources_iter)->release(*requested_resources_iter);
  }
  //TODO
  return true;
}
