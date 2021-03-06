// Copyright 2015 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "rclcpp/executors/single_threaded_executor.hpp"
#include "rclcpp/scope_exit.hpp"

using rclcpp::executors::single_threaded_executor::SingleThreadedExecutor;

SingleThreadedExecutor::SingleThreadedExecutor(const rclcpp::executor::ExecutorArgs & args)
: executor::Executor(args) {}

SingleThreadedExecutor::~SingleThreadedExecutor() {}

void
SingleThreadedExecutor::spin()
{
    
  std::lock_guard<std::mutex> lock(spin_lock);
  if (spinning.exchange(true)) {
    throw std::runtime_error("spin_some() called while already spinning");
  }
  
  RCLCPP_SCOPE_EXIT(this->spinning.store(false); );
  while (rclcpp::utilities::ok() && spinning.load()) {
    auto any_exec = get_next_executable();
    execute_any_executable(any_exec);
  }
}
