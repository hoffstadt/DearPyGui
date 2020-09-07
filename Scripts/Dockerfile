FROM debian:10-slim

RUN apt-get update &&\
    apt-get install --no-install-recommends -qy\
        git \
        build-essential \
        cmake \
        xorg-dev \
        libgl1-mesa-dev \
        python3-dev \
        python3-pip \
        python3-setuptools \
        python3-wheel \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

RUN useradd -m -d /home/dearpygui -s /bin/bash dearpygui

USER dearpygui
WORKDIR /app

CMD ["bash"]
