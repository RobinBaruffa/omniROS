
(cl:in-package :asdf)

(defsystem "model_state_stamper-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :geometry_msgs-msg
               :std_msgs-msg
)
  :components ((:file "_package")
    (:file "ModelStatesStamped" :depends-on ("_package_ModelStatesStamped"))
    (:file "_package_ModelStatesStamped" :depends-on ("_package"))
  ))