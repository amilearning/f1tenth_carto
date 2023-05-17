#include "ros/ros.h"
#include <cmath>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <nav_msgs/Odometry.h>
#include <cartographer_ros_msgs/FinishTrajectory.h>
#include <cartographer_ros_msgs/GetTrajectoryStates.h>
#include <cartographer_ros_msgs/StartTrajectory.h>
#include <cartographer_ros_msgs/StatusResponse.h>
#include <cartographer_ros_msgs/TrajectoryStates.h>


class CartoInitPose{
    public:
        CartoInitPose(){
            init_pose_sub = nh.subscribe("/initialpose",1,&CartoInitPose::poseCallBack,this);
            init_pose_mcl_sub = nh.subscribe("/pf/pose/odom/mcl",1,&CartoInitPose::poseCallBack_mcl,this);
        }
        /**
            @brief Get pose from Rviz(/initialpose)
        */
        void poseCallBack(geometry_msgs::PoseWithCovarianceStamped::ConstPtr in_pose_cov){
            end_last_trajectory();
            start_new_trajetory(in_pose_cov->pose.pose);
        }

        void poseCallBack_mcl(nav_msgs::Odometry::ConstPtr in_pose_cov){
            end_last_trajectory();
            start_new_trajetory(in_pose_cov->pose.pose);
        }

    private:

        ros::NodeHandle nh;
        ros::Subscriber init_pose_sub;
        ros::Subscriber init_pose_mcl_sub;

        /**
        @brief Get lastest trajectory number and finish it
        */
        void end_last_trajectory(){
            ros::ServiceClient traj_state_client = nh.serviceClient<cartographer_ros_msgs::GetTrajectoryStates>("/get_trajectory_states");
            cartographer_ros_msgs::GetTrajectoryStates traj_state_srv;
            
            int last_idx;
            if (traj_state_client.call(traj_state_srv)){
                last_idx = int(traj_state_srv.response.trajectory_states.trajectory_id.back());
            }
            else{
                ROS_ERROR("Failed to call service get_last_traj_state");
                return ;
            }

            ros::ServiceClient fin_traj_client = nh.serviceClient<cartographer_ros_msgs::FinishTrajectory>("/finish_trajectory");
            cartographer_ros_msgs::FinishTrajectory fin_traj_srv;

            fin_traj_srv.request.trajectory_id = last_idx;
            fin_traj_client.call(fin_traj_srv);
            
        }

        /**
            @brief Start new trajectory with given pose
        */
        void start_new_trajetory(geometry_msgs::Pose in_pose){
            ros::ServiceClient start_traj_client = nh.serviceClient<cartographer_ros_msgs::StartTrajectory>("/start_trajectory");
            cartographer_ros_msgs::StartTrajectory start_traj_srv;
            // Setting Values
            
            start_traj_srv.request.configuration_directory = "/home/ros2nx/carto_ws/src/cartographer_ros/cartographer_ros/configuration_files";
            start_traj_srv.request.configuration_basename = "backpack_2d_localization.lua";
            start_traj_srv.request.use_initial_pose = true;
            start_traj_srv.request.initial_pose = in_pose;
            start_traj_srv.request.relative_to_trajectory_id = 0;
            start_traj_client.call(start_traj_srv);
        }


    
};



int main(int argc, char** argv)
{
    ros::init(argc, argv, "cartoInitPose");
    
    CartoInitPose cip;
    
    ros::spin();
    
    return 0;
}