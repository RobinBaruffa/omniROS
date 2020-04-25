#!/bin/sh

if [ -n "$DESTDIR" ] ; then
    case $DESTDIR in
        /*) # ok
            ;;
        *)
            /bin/echo "DESTDIR argument must be absolute... "
            /bin/echo "otherwise python's distutils will bork things."
            exit 1
    esac
    DESTDIR_ARG="--root=$DESTDIR"
fi

echo_and_run() { echo "+ $@" ; "$@" ; }

echo_and_run cd "/home/user/omniROS/omniROS_ws/src/ROSSerial"

# ensure that Python install destination exists
echo_and_run mkdir -p "$DESTDIR/home/user/omniROS/omniROS_ws/install/lib/python2.7/dist-packages"

# Note that PYTHONPATH is pulled from the environment to support installing
# into one location when some dependencies were installed in another
# location, #123.
echo_and_run /usr/bin/env \
    PYTHONPATH="/home/user/omniROS/omniROS_ws/install/lib/python2.7/dist-packages:/home/user/omniROS/omniROS_ws/build/lib/python2.7/dist-packages:$PYTHONPATH" \
    CATKIN_BINARY_DIR="/home/user/omniROS/omniROS_ws/build" \
    "/usr/bin/python2" \
    "/home/user/omniROS/omniROS_ws/src/ROSSerial/setup.py" \
    build --build-base "/home/user/omniROS/omniROS_ws/build/ROSSerial" \
    install \
    $DESTDIR_ARG \
    --install-layout=deb --prefix="/home/user/omniROS/omniROS_ws/install" --install-scripts="/home/user/omniROS/omniROS_ws/install/bin"
