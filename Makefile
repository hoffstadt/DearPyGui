XSOCK?=/tmp/.X11-unix
XAUTH?=/tmp/.docker.xauth

PHONY: all release deps build_env distclean wheel up

all:
	$(MAKE) release
	$(MAKE) wheel
	echo "$@ built ok"


release:
	mkdir -p  cmake-build-release \
		&& cd cmake-build-release \
		&& cmake ..
	cmake --build cmake-build-release --config Release -- -j 10


wheel:
	cd Distribution \
		&& python3 BuildPythonWheel.py ../cmake-build-release/DearPyGui/dearpygui.so dearpygui.so \
		&& python3 setup.py bdist_wheel --plat-name manylinux1_x86_64 --dist-dir ../dist


embed: deps
	$(MAKE) all
	echo "$@ built ok"


deps:
	cd Dependencies/cpython \
		&& mkdir -p build \
		&& cd build \
		&& ../configure --enable-shared --prefix $(PWD) \
		&& $(MAKE)


build_env:
	docker build -t dearpygui Scripts


distclean:
	rm -rf ./cmake-build-release dist/*


up:
	touch ${XAUTH} \
	&& xauth nlist ${DISPLAY} | sed -e 's/^..../ffff/' | xauth -f ${XAUTH} nmerge - \
	&& docker run -it --volume=${XSOCK}:${XSOCK}:rw \
        --volume=${XAUTH}:${XAUTH}:rw \
		--volume=${PWD}:/app \
        --env="XAUTHORITY=${XAUTH}" \
        --env="DISPLAY" \
        --user="dearpygui" \
		dearpygui bash

up_dev:
	docker run -it -v $(PWD):/app dearpygui bash
