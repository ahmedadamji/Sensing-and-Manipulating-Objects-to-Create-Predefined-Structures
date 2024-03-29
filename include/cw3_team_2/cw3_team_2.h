/* Software License Agreement (MIT License)
 *
 *  Copyright (c) 2019-, Dimitrios Kanoulas
 *
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the copyright holder(s) nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef Cw3_TEAM_2_H_
#define Cw3_TEAM_2_H_

#define _USE_MATH_DEFINES

#include <ros/ros.h>
#include <ros/time.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>

// ROS includes
#include <std_msgs/String.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/PointStamped.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/Quaternion.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Scalar.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

// PCL specific includes
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/common/centroid.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/passthrough.h>
#include <pcl/filters/conditional_removal.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/features/normal_3d.h>
#include <pcl/ModelCoefficients.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/search/kdtree.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/segmentation/extract_clusters.h>
#include <pcl/conversions.h>
#include <pcl_ros/point_cloud.h>
#include <pcl/kdtree/kdtree.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/common/common.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/point_cloud_conversion.h>

#include <iostream>
#include <cstdlib>

// TF specific includes
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>

// standard c++ library includes (std::string, std::vector)
#include <string>
#include <vector>

// headers generated by catkin for the custom services we have made
#include <cw3_world_spawner/Task1Service.h>
#include <cw3_world_spawner/Task2Service.h>
#include <cw3_world_spawner/Task3Service.h>
#include <cw3_world_spawner/TaskSetup.h>


typedef pcl::PointXYZRGBA PointT;
typedef pcl::PointCloud<PointT> PointC;
typedef PointC::Ptr PointCPtr;

/** \brief Cw3 Solution.
  *
  * \author Ahmed Adamjee, Abdulbaasit Sanusi, Kennedy Dike
  */
class Cw3Solution
{
  public:

    /** \brief  Class constructor. 
      *
      * \input[in] nh ROS node handle
      */
    Cw3Solution(ros::NodeHandle& nh);


    /** \brief Point Cloud CallBack function.
      * 
      * \input[in] cloud_input a PointCloud2 sensor_msgs const pointer
      */
    void
    cloudCallBackOne (const sensor_msgs::PointCloud2ConstPtr& cloud_input_msg);

    /** \brief Service callback function for entire task 1 
      *
      * used for picking and placing at given position 
      * 
      * \input[in] request service request message
      * \input[in] response service response message
      *  
      * \return true if service succeeds
      */
    bool 
    task1Callback(cw3_world_spawner::Task1Service::Request &request,
      cw3_world_spawner::Task1Service::Response &response);

    /** \brief Service callback function for task 2 
      *
      * used for object detection and localization
      * 
      * \input[in] request service request message
      * \input[in] response service response message
      *  
      * \return true if service succeeds
      */
    bool 
    task2Callback(cw3_world_spawner::Task2Service::Request &request,
      cw3_world_spawner::Task2Service::Response &response);

      /** \brief Service callback function for task 3
      *
      * used for picking multiple objects of the same colour
      * 
      * \input[in] request service request message
      * \input[in] response service response message
      *  
      * \return true if service succeeds
      */
    bool 
    task3Callback(cw3_world_spawner::Task3Service::Request &request,
      cw3_world_spawner::Task3Service::Response &response);


    /** \brief Clearing the lists that store centroids and other information of any previous detected clusters from global variables.
      *
      * ...
      */
    void
    clearPreviousScanData();
    
    /** \brief function to scan the entire mat. Used in Task 3
      *
      * ...
      */
    void
    scanEntireMat();

    /** \brief function to scan the front of the mat. Used in Task 1 and 2
      *
      * ...
      */
    void
    scanFrontMat();

    /** \brief function to find and store centroid found during scanning
      *
      * ...
      */
    void
    findCentroidsAtScanLocation();

      
      /** \brief function to pick and place cube at particular centroid location
      *
      * \input[in] list of centroid specifying location to pick
      * \input[in] goal location
      * \return true if successful
      */
    bool
    pickaAndPlaceCube(std::vector<geometry_msgs::PointStamped> centroids, geometry_msgs::Point goal_loc);

    /** \brief function to specify scan area
      *
      * \input[in] container to store pose of scan area
      * \input[in] coordinate x of scan area
      * \input[in] coordinate y of scan area
      * \input[in] coordinate z of scan area
      * \return the coordinate of the scan area
      */
    geometry_msgs::Pose
    scan(geometry_msgs::Pose scan_num, float x, float y, float z);

    /** \brief function to specify collision origin
      *
      * \input[in] container to store origin of collision area
      * \input[in] coordinate x of origin
      * \input[in] coordinate y of origin
      * \input[in] coordinate z of origin
      * \return the coordinate of the origin
      */
    geometry_msgs::Point
    origin(geometry_msgs::Point collision_origin, float x, float y, float z);

    /** \brief function to specify collision object dimension
      *
      * \input[in] container to store dimension of collision object
      * \input[in] length x of dimension
      * \input[in] breadth y of dimension
      * \input[in] height z of dimension
      * \return the dimension of collision object
      */
    geometry_msgs::Vector3
    dimension(geometry_msgs::Vector3 collision_dimension, float x, float y, float z);

    /** \brief function to specify collision object orientation
      *
      * \input[in] container to store orientation of collision object
      * \input[in]  x of orientation
      * \input[in]  y of orientation
      * \input[in]  z of orientation
      * \input[in]  w of orientation
      * \return the orientation of collision object
      */

    geometry_msgs::Quaternion
    orientation(geometry_msgs::Quaternion collision_orientation, float x, float y, float z, float w);


    /** \brief MoveIt function for moving the move_group to the target position.
      *
      * \input[in] target_pose pose to move the arm to
      *
      * \return true if moved to target position 
      */
    bool 
    moveArm(geometry_msgs::Pose target_pose);

    /** \brief MoveIt function for moving the gripper fingers to a new position. 
      *
      * \input[in] width desired gripper finger width
      *
      * \return true if gripper fingers are moved to the new position
      */
    bool 
    moveGripper(float width);

    /** \brief MoveIt function for adding a cuboid collision object in RViz
      * and the MoveIt planning scene.
      *
      * \input[in] object_name name for the new object to be added
      * \input[in] centre point at which to add the new object
      * \input[in] dimensions dimensions of the cuboid to add in x,y,z
      * \input[in] orientation rotation to apply to the cuboid before adding
      */
    void
    addCollisionObject(std::string object_name, geometry_msgs::Point centre, 
      geometry_msgs::Vector3 dimensions, geometry_msgs::Quaternion orientation);

    /** \brief MoveIt function for adding a cuboid attached collision object in RViz
      * and the MoveIt planning scene which can work even with octomap activated.
      *
      * \input[in] object_name name for the new object to be added
      * \input[in] centre point at which to add the new object
      * \input[in] dimensions dimensions of the cuboid to add in x,y,z
      * \input[in] orientation rotation to apply to the cuboid before adding
      */
    void
    addAttachedCollisionObject(std::string object_name, geometry_msgs::Point centre, 
      geometry_msgs::Vector3 dimensions, geometry_msgs::Quaternion orientation);

    /** \brief Remove a collision object from the planning scene.
      * 
      * \input[in] object_name for the object to be removed
      */
    void 
    removeCollisionObject(std::string object_name);

    /** \brief Pick an object up with a given position.
      * 
      * \input[in] position the xyz coordinates where the gripper converges
      */
    bool
    pick(geometry_msgs::Point position);

    /** \brief Place an object up with a given position.
      * 
      * \input[in] position the xyz coordinates where the gripper converges
      */
    bool
    place(geometry_msgs::Point position);

    /** \brief Function to pick and place all the cubes to create a stack.
      * 
      *  \output true if all cubes have been deposited as requested
      */
    bool
    pickAndPlaceIndexedCubes();
    
    /** \brief Apply Voxel Grid filtering.
      * 
      * \input[in] in_cloud_ptr the input PointCloud2 pointer
      * \input[out] out_cloud_ptr the output PointCloud2 pointer
      */
    void
    applyVX (PointCPtr &in_cloud_ptr,
             PointCPtr &out_cloud_ptr);

    /** \brief Apply Pass Through filtering.
      * 
      * \input[in] in_cloud_ptr the input PointCloud2 pointer
      * \input[out] out_cloud_ptr the output PointCloud2 pointer
      */
    void
    applyPT (PointCPtr &in_cloud_ptr,
             PointCPtr &out_cloud_ptr);


    /** \brief Apply Floor filtering.
      * 
      * \input[in] in_cloud_ptr the input PointCloud2 pointer
      * \input[out] out_cloud_ptr the output PointCloud2 pointer
      */
    void
    applyFF (PointCPtr &in_cloud_ptr,
             PointCPtr &out_cloud_ptr);
    
    
    /** \brief Normal estimation.
      * 
      * \input[in] in_cloud_ptr the input PointCloud2 pointer
      */
    void
    findNormals (PointCPtr &in_cloud_ptr);
    
    /** \brief Segment Plane from point cloud.
      * 
      * \input[in] in_cloud_ptr the input PointCloud2 pointer
      */
    void
    segPlane (PointCPtr &in_cloud_ptr);

    
    /** \brief Extract inliers from input point cloud.
      * 
      * \input[in] in_cloud_ptr the input PointCloud2 pointer
      */
    void
    extractInlier (PointCPtr &in_cloud_ptr);

    /** \brief Segment clusters from point cloud.
      * 
      * \input[in] in_cloud_ptr the input PointCloud2 pointer
      */
    void
    segClusters (PointCPtr &in_cloud_ptr);

    /** \brief Find the Pose of Cube.
      * 
      * \input[in] in_cloud_ptr the input PointCloud2 pointer
      */
    geometry_msgs::PointStamped
    findCubePose (PointCPtr &in_cloud_ptr); //set return here
    
    /** \brief Point Cloud publisher.
      * 
      *  \input pc_pub ROS publisher
      *  \input pc point cloud to be published
      */
    void
    pubFilteredPCMsg (ros::Publisher &pc_pub, PointC &pc);
    
    /** \brief Publish the cube point.
      * 
      *  \input[in] cube_pt_msg Cube's geometry point
      *  
      *  \output true if three numbers are added
      */
    void
    publishPose (geometry_msgs::PointStamped &cube_pt_msg);



      
    /* Variables */

    /** \brief Define object and goal locations of objects to manipulate */
    //geometry_msgs/PoseStamped object_loc;

    /** \brief Define some useful constant values */
    std::string base_frame_ = "panda_link0";
    double gripper_open_ = 80e-3;
    double gripper_closed_ = 0.0;

    /** \brief Parameters to define the pick operation */
    double z_offset_ = 0.125;
    double angle_offset_ = 3.14159 / 4.0;
    double approach_distance_ = 0.1;
    
    /** \brief Node handle. */
    ros::NodeHandle g_nh;

    /** \brief  service servers for advertising ROS services  */
    ros::ServiceServer task1_srv_;
    ros::ServiceServer task2_srv_;
    ros::ServiceServer task3_srv_;


    /** \brief MoveIt interface to move groups to seperate the arm and the gripper,
      * these are defined in urdf. */
    moveit::planning_interface::MoveGroupInterface arm_group_{"panda_arm"};
    moveit::planning_interface::MoveGroupInterface hand_group_{"hand"};

    /** \brief MoveIt interface to interact with the moveit planning scene 
      * (eg collision objects). */
    moveit::planning_interface::PlanningSceneInterface planning_scene_interface_;


    /** \brief The input point cloud frame id. */
    std::string g_input_pc_frame_id_;

    /** \brief ROS publishers. */
    ros::Publisher g_pub_cloud;

    /** \brief ROS subscribers. */
    ros::Subscriber g_sub_cloud;
    
    /** \brief ROS geometry message point. */
    geometry_msgs::PointStamped g_cube_pt_msg;

    /** \brief Current centroid found */
    geometry_msgs::PointStamped g_current_centroid;

    /** \brief Max point of current cluster found */
    geometry_msgs::Point g_current_cluster_max;

    /** \brief Min point of current cluster found */
    geometry_msgs::Point g_current_cluster_min;
    
    /** \brief Stores number of cubes found in stack */
    int g_number_of_cubes_in_stack;

    /** \brief Stores number of cubes found in recorded stack */
    int g_number_of_cubes_in_recorded_stack;

    /** \brief Stores y coordinate corresponding to the max x bound of centroid */
    double g_current_cluster_max_x_y;

    /** \brief Stores x coordinate corresponding to the max y bound of centroid */
    double g_current_cluster_max_y_x;


    /** \brief All visbile centroids found */
    std::vector<geometry_msgs::PointStamped> g_centroids;

    /** \brief Colours of all cubes in the stack */
    std::vector<std_msgs::ColorRGBA> g_current_stack_colours;


    /** \brief All max points of visible centroids found */
    std::vector<geometry_msgs::Point> g_clusters_max;

    /** \brief All min points of visible centroids found */
    std::vector<geometry_msgs::Point> g_clusters_min;

    /** \brief Colours of all cubes in the stack */
    std::vector<std_msgs::ColorRGBA> g_colors;

    /** \brief Total number of points used to find colours of all cubes in the stack, used to find average colour value, used to find average rgb value */
    std::vector<int> g_colors_count;

    /** \brief Colour of current cube found */
    std_msgs::ColorRGBA g_current_color;

    /** \brief Total number of points used to find colour of current cube found, used to find average rgb value */
    int g_current_color_count;

    /** \brief Stores the total number of pixels for colours found for all the scanned cubes in the stack */
    std::vector<int> g_current_stack_cube_color_count;

    /** \brief ROS pose publishers. */
    ros::Publisher g_pub_pose;
    
    /** \brief Voxel Grid filter's leaf size. */
    double g_vg_leaf_sz;
    
    /** \brief Point Cloud (input) pointer. */
    PointCPtr g_cloud_ptr;
    
    /** \brief Point Cloud (filtered) pointer. */
    PointCPtr g_cloud_filtered, g_cloud_filtered2;
    
    /** \brief Point cloud to hold plane and cylinder points. */
    PointCPtr g_cloud_plane;
    
    /** \brief Point Cloud (filtered) sensros_msg for publ. */
    sensor_msgs::PointCloud2 g_cloud_filtered_msg;
    
    /** \brief Point Cloud (input). */
    pcl::PCLPointCloud2 g_pcl_pc;
    
    /** \brief Voxel Grid filter. */
    pcl::VoxelGrid<PointT> g_vx;
    
    /** \brief Pass Through filter. */
    pcl::PassThrough<PointT> g_pt;
    
    /** \brief  Min and Max y threshold sizes. */
    double g_y_thrs_min, g_y_thrs_max;
    
    /** \brief Min and Max x threshold sizes. */
    double g_x_thrs_min, g_x_thrs_max;
    
    /** \brief Color filter. */
    pcl::ConditionalRemoval<PointT> g_cf;
    
    /** \brief Floor Filtering. */
    pcl::ConditionalRemoval<PointT> g_ff;

    /** \brief Color filter rgb filter values. */
    double g_cf_red, g_cf_green, g_cf_blue;
    
    /** \brief KDTree for nearest neighborhood search. */
    pcl::search::KdTree<PointT>::Ptr g_tree_ptr;
    
    /** \brief Normal estimation. */
    pcl::NormalEstimation<PointT, pcl::Normal> g_ne;
    
    /** \brief Cloud of normals. */
    pcl::PointCloud<pcl::Normal>::Ptr g_cloud_normals, g_cloud_normals2;
    
    /** \brief Nearest neighborhooh size for normal estimation. */
    double g_k_nn;
    
    /** \brief SAC segmentation. */
    pcl::SACSegmentationFromNormals<PointT, pcl::Normal> g_seg; 
    
    /** \brief Euclidean Cluster Extraction. */
    pcl::EuclideanClusterExtraction<PointT> g_ec;
    
    /** \brief Extract point cloud indices. */
    pcl::ExtractIndices<PointT> g_extract_pc;
  
    /** \brief Extract point cloud normal indices. */
    pcl::ExtractIndices<pcl::Normal> g_extract_normals;
    
    /** \brief Point indices for plane. */
    pcl::PointIndices::Ptr g_inliers_plane;
      
    
    /** \brief Model coefficients for the plane segmentation. */
    pcl::ModelCoefficients::Ptr g_coeff_plane;
    
    /** \brief cw3Q1: TF listener definition. */
    tf::TransformListener g_listener_;
    
    /** \brief Stores indices of point cloud for each cluster */
    std::vector<pcl::PointIndices> g_cluster_indices;
    
    /** \brief Stores all centroids found for the requested scan */
    std::vector<geometry_msgs::PointStamped> centroids;

    std::vector<geometry_msgs::PointStamped> g_oldcentroids;

    
    /** \brief Stores all max points of centroids found for the requested scan */
    std::vector<geometry_msgs::Point> clusters_max;
    
    /** \brief Stores all min points of centroids found for the requested scan */
    std::vector<geometry_msgs::Point> clusters_min;


    /** \brief Stores all colours found for the scanned cubes */
    std::vector<std_msgs::ColorRGBA> colors;

    /** \brief Stores the total number of pixels for colours found for all the scanned cubes */
    std::vector<int> colors_count;


    /** \brief Stores the temperory rgba values of the cubes */
    std_msgs::ColorRGBA g_Color;

    /** \brief Sets the lower bound threshold for scanning points in the world frame */
    geometry_msgs::PointStamped g_pt_world_lb;
    
    /** \brief Sets the upper bound threshold for scanning points in the world frame */
    geometry_msgs::PointStamped g_pt_world_ub;


    /** \brief Sets a flag to read the rgb values of points of a stack of cubes in the cloudCallbackOne function when required */
    bool g_check_objects_stack = false;

    /** \brief Sets a flag to read the rgb values of points of objects lying on the floor in the cloudCallbackOne function when required */
    bool g_check_objects_floor = false;


    /** \brief Stores the variable name of object that needs to be picked, to add a collision object once it has been deposited, to add them in the planning scene */
    std::string g_pick_object = "object";

    /** \brief Stores the variable name of collision objects detected, to add them in the planning scene */
    std::string g_collision_object = "object";


    /** \brief Stores the variable names of all objects that need to be picked, to add a collision object for each once it has been deposited, to add them in the planning scene */
    std::vector<std::string> g_pick_objects;


    /** \brief Stores the yaw value required to place objects in the requested location */
    float g_place_angle_offset_;


    /** \brief Stores number of cubes found on the floor for task 2 and 3*/
    int g_size = 0;

    /** \brief Stores the eucledian distance of a centroid found and a point of interest, to identify if the centroid lies at the point of interest*/
    double eu_distance;

    /** \brief Stores the computed orientation for all the clusters found on the floor*/
    std::vector<double> g_yaw_list;

    /** \brief Stores the corresponding y value of all the visible clusters, to the maximum x value found, used to find the orientation of each cluster*/
    std::vector<double> g_clusters_max_y_x;

    /** \brief Stores the corresponding x value of all the visible clusters, to the maximum y value found, used to find the orientation of each cluster*/
    std::vector<double> g_clusters_max_x_y;

    /** \brief Stores the corresponding y value of all the clusters, to the maximum x value found, used to find the orientation of each cluster*/
    std::vector<double> clusters_max_y_x;

    /** \brief Stores the corresponding x value of all the clusters, to the maximum y value found, used to find the orientation of each cluster*/
    std::vector<double> clusters_max_x_y;

    /** \brief Stores the boolean result regarding if a picking task has been successful*/
    bool g_pick_success;
    /** \brief Stores the boolean result regarding if a placing task has been successful*/
    bool g_place_success;
    /** \brief Stores the boolean result regarding if a moving task has been successful*/
    bool g_move_success;

    /** \brief this is used in defining the origin of the box collision object*/
    geometry_msgs::Point box_origin;

    /** \brief this is used in defining the dimension of the box collision object*/
    geometry_msgs::Vector3 box_dimension;

    /** \brief this is used in defining the orientation of the box collision object*/
    geometry_msgs::Quaternion box_orientation;


    /** \brief this is used to store the idices of the cubes found in order they need to be scanned*/
    std::vector<int> g_index_of_cubes_to_stack;

    /** \brief this is used to store the total number of cubes that need to be stacked*/
    int g_num_of_cubes_to_stack;

    /** \brief this is used to store the indices of all the objects found in the environment that are supposed to be collision object*/
    std::vector<int> g_index_of_collision_objects;

    /** \brief this is used to store the placing location of picked objects*/
    geometry_msgs::Point g_target_point;

    /** \brief this is used to store the height of all the clusters found in the environment*/
    std::vector<double> height_vector;

    /** \brief this is used to store the index of the stack from all the clusters found while scanning the environment*/
    int stack_index;



  protected:
    /** \brief Debug mode. */
    bool debug_;
};
#endif