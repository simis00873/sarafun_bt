#include <ros/ros.h>
#include <sarafun_tree/demo_bt_nodes/ApproachObjectsAction.h>
#include <sarafun_tree/demo_bt_nodes/GrabObjectAction.h>
#include <sarafun_tree/demo_bt_nodes/FoldingAssemblyAction.h>
#include <sarafun_tree/demo_bt_nodes/InsertionWithDeformationAction.h>
#include <sarafun_tree/demo_bt_nodes/PlaceAction.h>
#include <sarafun_tree/TreeRunner.h>
#include <sarafun_tree/LoadTree.h>
#include <std_srvs/Empty.h>

using namespace sarafun;
using namespace BT;

TreeRunner *runner = 0;
bool running = false;
int TickPeriod_milliseconds = 0;

/*
  Initializes the tree runner.
*/
bool initializeTree(const std::string &tree_description_path)
{
  if (!running)
  {
    if (runner == 0)
    {
      runner = new TreeRunner(TickPeriod_milliseconds);
    }

    if(runner->startTree(tree_description_path))
    {
      running = true;
      ROS_INFO("Tree started successfully!");
      return true;
    }
    else
    {
      running = false;
      ROS_ERROR("Tree failed to start!");
      return false;
    }
  }

  ROS_ERROR("Tree already initialized");
  return false;
}

/*
  Stops and unloads the tree runner
*/
bool unloadTree()
{
  if (running)
  {
    runner->stopTree();
    running = false;
    return true;
  }

  return false;
}

/*
  Starts behavior tree execution
*/
bool startTreeCallback(sarafun_tree::LoadTree::Request &req, sarafun_tree::LoadTree::Response &ans)
{
  ans.success = initializeTree(req.file_path);
  return true;
}

/*
  Stops behavior tree execution
*/
bool stopTreeCallback(std_srvs::Empty::Request &req, std_srvs::Empty::Response &ans)
{
  if (!unloadTree())
  {
    ROS_ERROR("Called the service to stop tree without having started it");
    return false;
  }

  ROS_INFO("Tree stopped successfully");
  return true;
}

/*
  Restarts behavior tree execution
*/
bool restartTreeCallback(sarafun_tree::LoadTree::Request &req, sarafun_tree::LoadTree::Response &ans)
{
  if (!unloadTree())
  {
    ROS_ERROR("Called the service to stop tree without having started it");
    ans.success = false;
    return false;
  }
  else
  {
    ans.success = initializeTree(req.file_path);
    return true;
  }
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "sarafun_bt_demo");
  ros::NodeHandle n;

  if (ros::param::has("/sarafun/bt/tick_period")){
	ros::param::get("/sarafun/bt/tick_period", TickPeriod_milliseconds);
  } else {
    TickPeriod_milliseconds = 1000;
  }

  ros::ServiceServer bt_start_service = n.advertiseService("/sarafun/start_tree", startTreeCallback);
  ros::ServiceServer bt_stop_service = n.advertiseService("/sarafun/stop_tree", stopTreeCallback);
  ros::ServiceServer bt_restart_service = n.advertiseService("/sarafun/restart_tree", restartTreeCallback);

  ROS_INFO("Started the sarafun tree node!");
  ros::spin();
  return 0;
}
