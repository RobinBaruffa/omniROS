#!/usr/bin/env python
import rospy
import Tkinter as tk
import math
from geometry_msgs.msg import Twist



class GUIobject(tk.Tk):
	def __init__(self):
		rospy.init_node('gui_node', anonymous=False)
		#rate = rospy.Rate(300) # 10hz
		global pub

		global TRANSLATION_SPEED_MAX
		global ROTATION_SPEED_MAX
		global CANVAS_SIZE
		global GUI_PUBLISHER_TOPIC_NAME

		TRANSLATION_SPEED_MAX = rospy.get_param('/Translation_speed_max', 0.002)
		ROTATION_SPEED_MAX = rospy.get_param('/Rotation_speed_max',0.1)
		CANVAS_SIZE = rospy.get_param('/Gui_size',500)
		GUI_PUBLISHER_TOPIC_NAME = rospy.get_param('/Gui_publisher_topic_name','/omniROS/cmd_vel')

		print("Parameter CANVAS SIZE = ",CANVAS_SIZE)
		pub = rospy.Publisher(GUI_PUBLISHER_TOPIC_NAME, Twist, queue_size=1)
		tk.Tk.__init__(self)
		self.wheelCount = 0
		self.canvas = tk.Canvas(self, width=CANVAS_SIZE, height=CANVAS_SIZE)
		self.canvas.pack(side="top", fill="both", expand=True)
		self.canvas.create_line(CANVAS_SIZE/2,CANVAS_SIZE/2, CANVAS_SIZE/2,CANVAS_SIZE/2, tags=("line",), arrow="last",fill="red",width="3")
		self.text_id_x=self.canvas.create_text(350,10,fill="red",text="")
		self.text_id_y=self.canvas.create_text(350,30,fill="green",text="")
		self.wheelCount = 0
		self.text_id_z=self.canvas.create_text(350,50,fill="blue",text="")
		self.bind("<Button-4>", self._on_mousewheel_plus)
		self.bind("<Button-5>", self._on_mousewheel_minus)
		self.rotate()

	def _on_mousewheel_plus(self,event):
		self.wheelCount += 1

	def _on_mousewheel_minus(self,event):
		self.wheelCount -= 1

	def rotate(self):
		msg = Twist()

		x = self.winfo_pointerx() - self.winfo_rootx()  #set relative coordinates
		y = self.winfo_pointery() - self.winfo_rooty()

		if x> CANVAS_SIZE:
			x=CANVAS_SIZE
		if x< 0:
			x=0
		if y> CANVAS_SIZE:
			y=CANVAS_SIZE
		if y<0:
			y=0

		self.canvas.coords("line", CANVAS_SIZE/2,CANVAS_SIZE/2,x,y)

		if(x< CANVAS_SIZE and y< CANVAS_SIZE):
			msg.linear.x=(x-CANVAS_SIZE/2)/(0.5*CANVAS_SIZE)*TRANSLATION_SPEED_MAX
			msg.linear.y=-(y-CANVAS_SIZE/2)/(0.5*CANVAS_SIZE)*TRANSLATION_SPEED_MAX
			msg.angular.z = self.wheelCount*ROTATION_SPEED_MAX
		else:
			msg.linear.x=0
			msg.linear.y=0
			msg.angular.z=0
		self.canvas.itemconfig(self.text_id_x,text=str(msg.linear.x)+ " m/s")
		self.canvas.itemconfig(self.text_id_y,text=str(msg.linear.y)+ " m/s")
		self.canvas.itemconfig(self.text_id_z,text=str(msg.angular.z)+ " rad/s")

		talker(msg)
		self.after(30, self.rotate)


def talker(msg):
    if not rospy.is_shutdown():
	try:
		pub.publish(msg)
	except rospy.ServiceException:
		print "Call service fail"
	#rospy.loginfo(msg)


if __name__ == '__main__':
    try:
	app = GUIobject()
	app.mainloop()
    except rospy.ROSInterruptException:
        pass
