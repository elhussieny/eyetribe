# Eyetribe ROS Client using VM
***A ROS Interface for the Eyetrbe device using VirtualMachine.***

Till this moment there is no driver for the [Eyetribe](http://theeyetribe.com/) device on Linux. The work around solution is to interface it through a VM, either using Virtualbox or VMPlayer. The tested host is Ubuntu 14.04LTS and the VM guest should supports USB3.0. [This link](http://forum.cogsci.nl/index.php?p=/discussion/1590/using-the-eyetribe-on-linux-ubuntu-through-virtualbox/p1) may help.
If the Eyetribe not appeared at the end you can do these steps:
  
  1. Enable USB3.0 from Ubuntu:
      $ sudo usermod -a -G vboxusers [username]
  2. Increase the resources for the VM guest such as RAM, CPU and VGA memory.  
# To Build the ROS Eyetribe Client:

    cd ~ROS_Workspace/src/
<<<<<<< HEAD
    git clone http://github.com/elhussieny/eyetribe.git
    cd ~/ROS_Workspace/
=======
    
    git clone http://github.com/elhussieny/eyetribe.git
    
    cd ~/ROS_Workspace/
    
>>>>>>> a300d90ef23b850aefaccea3caf8029d58db0627
    catkin_make

# To Run The ROS Eyetribe Client: 

    roscore
    rosrun eyetribe eyetribe_client
#Note:
The code uses the xdotool to move the mouse to the specified gaze location. The resolution of the screen could be changed with the #defined SCREEN_V_RES and SCREEN_H_RES. 
