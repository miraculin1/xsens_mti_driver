#ifndef STATUSPUBULISHER_H
#define STATUSPUBULISHER_H

#include "packetcallback.h"
#include <geometry_msgs/Vector3Stamped.h>

struct StatusPublisher : public PacketCallback
{
    /* data */
    ros::Publisher pub;
    std::string frame_id = DEFAULT_FRAME_ID;

    StatusPublisher(ros::NodeHandle &node)
    {
        int pub_queue_size = 5;
        ros::param::get("~publisher_queue_size", pub_queue_size);
        pub = node.advertise<geometry_msgs::Vector3Stamped>("imu/status", pub_queue_size);
        ros::param::get("~frame_id", frame_id);
    }

    void operator()(const XsDataPacket &packet, ros::Time timestamp)
    {
        if (packet.containsStatus())
        {
            geometry_msgs::Vector3Stamped msg;

            msg.header.stamp = timestamp;
            msg.header.frame_id = frame_id;

            uint32_t s = packet.status();
            if (s & (1 << 22))
                msg.vector.x = 1;
            else
                msg.vector.x = 0;

            pub.publish(msg);
        }
    }
};

#endif