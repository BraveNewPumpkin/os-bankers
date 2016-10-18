//
// Created by Kyle Bolton on 10/6/16.
//

#include "Bank.h"

const unsigned int Bank::addResource(const unsigned int& num_instances) {
  unsigned int index = resources.size();
  resources.push_back(std::make_unique<Resource>(index, num_instances));
  return index;
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

void Bank::releaseAllResourcesForProcess(const std::unique_ptr<Process> &process) {
  std::vector<unsigned int>::iterator debits_for_process_iter;
  std::vector<std::unique_ptr<Resource> >::iterator resource_ptr_iter;
  for(
    debits_for_process_iter = debits->at(process->getIndex()).begin(), resource_ptr_iter = resources.begin();
    debits_for_process_iter != debits->at(process->getIndex()).end() && resource_ptr_iter != resources.end();
    ++debits_for_process_iter, ++resource_ptr_iter
     ){
    (*resource_ptr_iter)->release(*debits_for_process_iter);
  }
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
  }
  std::vector<std::vector<unsigned int> >::iterator claims_iter;
  std::vector<std::vector<unsigned int> >::iterator debits_iter;
  std::vector<unsigned int>::iterator claim_resource_iter;
  std::vector<unsigned int>::iterator debit_resource_iter;
  std::vector<unsigned int> resources_in_escrow(debits->size(), 0);
  std::vector<unsigned int>::iterator escrow_iter;
  std::vector<bool> approved(debits->size(), false);
  std::vector<bool>::iterator approved_iter;
  bool no_changes = false;
  while(!no_changes){
    no_changes = true;
    for(
       claims_iter = claims->begin(), debits_iter = debits->begin(), approved_iter = approved.begin();
       claims_iter != claims->end();
       ++claims_iter, ++debits_iter, ++approved_iter
       ) {
      if(*approved_iter == false) {
        for (
         requested_resources_iter = requested_resources.begin(), resources_iter = resources.begin(), claim_resource_iter = claims_iter->begin(), debit_resource_iter = debits_iter->begin(), escrow_iter = resources_in_escrow.begin();
         resources_iter != resources.end();
         ++requested_resources_iter, ++resources_iter, ++claim_resource_iter, ++debit_resource_iter, ++escrow_iter
         ) {
          const unsigned int requested_resource_amount = *requested_resources_iter;
          const unsigned int resource_available_amount = (*resources_iter)->getAvailable();
          const unsigned int claim_amount = *claim_resource_iter;
          const unsigned int debit_amount = *debit_resource_iter;
          const unsigned int escrow_amount = *escrow_iter;
          const int need = claim_amount - debit_amount;

          if(need <= resource_available_amount - requested_resource_amount + escrow_amount){
            *escrow_iter += debit_amount;
            *approved_iter = true;
            no_changes = false;
          }
        }
      }
    }
  }
  bool is_approved = true;
  for(const bool& is_passing: approved){
    is_approved = is_approved && is_passing;
  }
  return is_approved;
}
