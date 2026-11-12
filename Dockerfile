FROM ubuntu:24.04

ARG DEBIAN_FRONTEND=noninteractive
ARG CMAKE_VERSION=4.4.5
ARG GTEST_VERSION=v1.15.2

RUN apt-get update && apt-get -y install tzdata

RUN apt-get update && apt-get install -y --no-install-recommends \
        build-essential \
        gcc \
        g++ \
        gdb \
        clang \
        make \
        ninja-build \
        autoconf \
        automake \
        libtool \
        valgrind \
        locales-all \
        dos2unix \
        rsync \
        git \
        wget \
        ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Recent CMake from the official Kitware binary release
RUN ARCH="$(uname -m)" \
    && wget -q "https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-linux-${ARCH}.sh" -O /tmp/cmake.sh \
    && sh /tmp/cmake.sh --skip-license --prefix=/usr/local \
    && rm /tmp/cmake.sh \
    && for t in cmake ctest cpack ccmake; do ln -sf /usr/local/bin/$t /usr/bin/$t; done

# Recent GoogleTest (includes GoogleMock), built and installed system-wide
RUN git clone --depth 1 --branch "${GTEST_VERSION}" https://github.com/google/googletest.git /tmp/googletest \
    && cmake -S /tmp/googletest -B /tmp/googletest/build -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_GMOCK=ON \
        -DCMAKE_INSTALL_PREFIX=/usr/local \
    && cmake --build /tmp/googletest/build \
    && cmake --install /tmp/googletest/build \
    && rm -rf /tmp/googletest

WORKDIR /workspace
CMD ["/bin/bash"]
