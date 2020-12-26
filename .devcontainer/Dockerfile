FROM ubuntu:20.04

ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y cmake cppcheck dpkg-dev valgrind sudo git-core g++ && \
    apt-get install -y libopencv-dev libtesseract-dev tesseract-ocr-eng tesseract-ocr-rus && \
    apt-get install -y googletest googletest-tools libgtest-dev && \
    apt-get clean && \
    groupadd --gid $USER_GID $USERNAME && \
    adduser --shell /bin/bash --uid $USER_UID --gid $USER_GID $USERNAME && \
    echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME && \
    chmod 0440 /etc/sudoers.d/$USERNAME


USER $USERNAME

# WORKDIR /home/developer/project
