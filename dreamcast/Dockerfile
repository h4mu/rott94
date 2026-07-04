FROM debian:bullseye

RUN apt-get update && apt-get install -y \
    build-essential \
    bash \
    git \
    texinfo \
    bison \
    flex \
    gettext \
    wget \
    curl \
    libgmp-dev \
    libmpfr-dev \
    libmpc-dev \
    libelf-dev \
    libjpeg-dev \
    libpng-dev \
    python3 \
    cmake \
    pkg-config

# Set up toolchain directory
RUN mkdir -p /opt/toolchains/dc

# Clone KOS and toolchain (or copy from local if we want to use the submodules)
# For the Dockerfile to be self-contained, it's better to clone or COPY.
# Since we have submodules, we can COPY them.
COPY . /src/rott-dreamcast

WORKDIR /src/rott-dreamcast/dreamcast/KOS/utils/kos-chain
RUN make build platform=dreamcast toolchain_profile=stable

# Build KOS
WORKDIR /src/rott-dreamcast/dreamcast/KOS
RUN cp doc/environ.sh.sample environ.sh
# Note: sourcing environ.sh in Docker RUN is tricky.
# We'll use a wrapper or manually set ENV.
ENV KOS_BASE=/src/rott-dreamcast/dreamcast/KOS
ENV KOS_ARCH=dreamcast
ENV KOS_CC_BASE=/opt/toolchains/dc/sh-elf
ENV PATH="${KOS_CC_BASE}/bin:${PATH}"

# We need to properly set up KOS environment to build it
RUN /bin/bash -c "source /src/rott-dreamcast/dreamcast/KOS/doc/environ.sh.sample && make"

# Build SDL2
WORKDIR /src/rott-dreamcast/dreamcast/SDL
RUN /bin/bash -c "source /src/rott-dreamcast/dreamcast/KOS/doc/environ.sh.sample && \
    mkdir build && cd build && \
    ../configure --host=sh-elf --prefix=\$KOS_BASE/addons && \
    make && make install"

# Build SDL2_mixer
WORKDIR /src/rott-dreamcast/dreamcast/SDL_mixer
RUN /bin/bash -c "source /src/rott-dreamcast/dreamcast/KOS/doc/environ.sh.sample && \
    mkdir build && cd build && \
    ../configure --host=sh-elf --with-sdl-prefix=\$KOS_BASE/addons --disable-music-midi --disable-music-mod --disable-music-mp3 --prefix=\$KOS_BASE/addons && \
    make && make install"

WORKDIR /src/rott-dreamcast
CMD ["/bin/bash"]
