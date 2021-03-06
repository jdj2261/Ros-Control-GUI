/**
 * @file /include/qtros/qnode.hpp
 *
 * @brief Communications central!
 *
 * @date February 2011
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef qtros_QNODE_HPP_
#define qtros_QNODE_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

// To workaround boost/qt4 problems that won't be bugfixed. Refer to
//    https://bugreports.qt.io/browse/QTBUG-22829
#ifndef Q_MOC_RUN
#include <ros/ros.h>
#endif
#include <string>
#include <QThread>
#include <QStringListModel>
#include <QLineEdit>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>

#include <move_base_msgs/MoveBaseActionGoal.h>
#include <actionlib_msgs/GoalID.h>
#include <nav_msgs/Odometry.h>
#include <std_srvs/Empty.h>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace qtros {

/*****************************************************************************
** Class
*****************************************************************************/

class QNode : public QThread {
  Q_OBJECT
public:
  QNode(int argc, char** argv );
  virtual ~QNode();
  bool init();
  bool init(const std::string &master_url, const std::string &host_url);
  void run();

  /*********************
  ** Logging
  **********************/
  enum LogLevel
  {
    Debug, Info, Warn, Error,Fatal
  };

  QStringListModel* loggingModel()
  {
    return &logging_model;
  }

  // Publish
  void sendCmdVelMsg(geometry_msgs::Twist msg);
  void sendGoalMsg(move_base_msgs::MoveBaseActionGoal msg);
  void sendSimpleGoalMsg(geometry_msgs::PoseStamped msg);
  void sendCancelGoalMsg(actionlib_msgs::GoalID msg);
  void sendClearCostmapSrv(std_srvs::Empty msg);

  // Log
  void log( const LogLevel &level, const std::string &msg);


  // Callback
  void OdomCallback(const nav_msgs::Odometry &msg);
  void InitialPoseCallback(const geometry_msgs::PoseWithCovarianceStamped &msg);
  void GoalPoseCallback(const move_base_msgs::MoveBaseActionGoal &msg);

  float m_pose_x, m_pose_y, m_pose_z, m_pose_w;
  float m_vel_x, m_vel_y, m_vel_z, m_vel_w;
  float m_init_x, m_init_y, m_init_z, m_init_w;
  float m_goal_x, m_goal_y, m_goal_z, m_goal_w;

Q_SIGNALS:
  void loggingUpdated();
  void rosShutdown();
  void OdomUpdated(float);
  void InitUpdated(float);
  void GoalUpdated(float);


private:
  int init_argc;
  char** init_argv;

  ros::Publisher chatter_publisher_;
  ros::Publisher cmd_vel_pub_;
  ros::Publisher goal_pose_pub_;
  ros::Publisher simple_goal_pub_;
  ros::Publisher cancel_goal_pub_;

  ros::Subscriber odom_sub_;
  ros::Subscriber init_pose_sub_;
  ros::Subscriber goal_pose_sub_;

  ros::ServiceClient clearCostmap_cient_;


  QStringListModel logging_model;

};

}  // namespace qtros

#endif /* qtros_QNODE_HPP_ */
