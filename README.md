# Eyetribe ROS Client using VM
***A ROS Interface for the Eyetrbe device using VirtualMachine.***

Till this moment there is no driver for the [Eyetribe](http://theeyetribe.com/) device on Linux. The work around solution is to interface it through a VM, either using Virtualbox or VMPlayer. The tested host is Ubuntu 14.04LTS and the VM guest should supports USB3.0. [This link](http://forum.cogsci.nl/index.php?p=/discussion/1590/using-the-eyetribe-on-linux-ubuntu-through-virtualbox/p1) may help.
If the Eyetribe not appeared at the end you can do these steps:
  
  1. Enable USB3.0 from Ubuntu:
      $ sudo usermod -a -G vboxusers [username]
  2. Increase the resources for the VM guest such as RAM, CPU and VGA memory.

# To Build the ROS Eyetribe Client on the host:

    cd ~ROS_Workspace/src/
    git clone http://github.com/elhussieny/eyetribe.git
    cd ~/ROS_Workspace/
    catkin_make

# To Run The ROS Eyetribe Client on the host: 

    roscore
    rosrun eyetribe eyetribe_client

# To Run Using launch File:
    roslaunch eyetribe eyetribe_client.launch

# Operational Parameters:
  In the launch file you can adjust the running parameters as follows:

  (1).

    <param name="v_reselutione" value="1080" />

The screen vertical resolution. Default is 1080.

  (2).

    <param name="h_reselutione" value="1920" />

The screen horizontal resolution. Default is 1920.

  (3).

    <param name="ip_server" value="192.168.152.128" />

The IP of the guest VM which runs the Eyetribe server. 

  (4).

    <param name="port_no" value="6555" />

The PortNo specified for the Eyetribe server. 

  (5).

    <param name="filter_coeff" value="0.1" />

The double exponential filter coeff. It is between 0 to 1. If small value selected, the gaze points will be highly filtered but with slow response and vise versa. 

# Notes:
(1) The code uses the xdotool to move the mouse pointer to the specified gaze location.
(2) To aquire gaze location continously from the Eyetribe server, the program implments a heartbeat that send a request every one sec.
(3) To enable the remote connection for the Eyetribe server by default, you need to add this EyeTribe.cfg file in the Eyetribe installation folder in Windows under:

	C:\Users\<current_user>\AppData\EyeTribe\EyeTribe.cfg

    {
    "config" : {
    "device" : 0,
    "remote" : true,
    "framerate" : 30,
    "port" : 6555   
    }
    }
