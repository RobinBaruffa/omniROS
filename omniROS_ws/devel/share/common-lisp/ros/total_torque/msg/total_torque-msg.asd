
(cl:in-package :asdf)

(defsystem "total_torque-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "Float32Stamped" :depends-on ("_package_Float32Stamped"))
    (:file "_package_Float32Stamped" :depends-on ("_package"))
  ))