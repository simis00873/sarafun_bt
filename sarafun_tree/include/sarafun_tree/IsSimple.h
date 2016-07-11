#ifndef __IS_SIMPLE_CONDITION__
#define __IS_SIMPLE_CONDITION__

#include <ros/ros.h>
#include <behavior_tree_leaves/ConditionTemplate.h>

namespace sarafun {
class IsSimple : ConditionTemplate{
public:
	IsSimple(std::string node_name, std::string bt_name) : ConditionTemplate(bt_name){
		node_handle_ = ros::NodeHandle(node_name);

		counter = 0;
		ros::param::get(node_name + "/count_limit", condition_limit);
  }
  ~IsSimple() {}

  bool conditionEvaluation();

private:
  ros::NodeHandle node_handle_;
  std::string node_name_;
  std::string bt_name_;

  int counter, condition_limit;
};
}
#endif
