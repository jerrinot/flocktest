FROM ubuntu:23.04

RUN DEBIAN_FRONTEND="noninteractive" apt-get update && apt-get -y install tzdata

RUN apt-get update \
  && apt-get install -y build-essential \
      gcc \
      make \
      cmake \
      autoconf \
      automake \
      libtool \
      locales-all \
  && apt-get clean

RUN mkdir /devel
RUN mkdir /var/shared
WORKDIR /devel
COPY ./main.c /devel/main.c
COPY ./CMakeLists.txt /devel/CMakeLists.txt
RUN cmake -S /devel -B /devel/build
RUN cmake --build /devel/build

ENTRYPOINT ["/devel/build/flocktest"]