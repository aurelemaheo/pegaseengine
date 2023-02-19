FROM ubuntu:22.04
COPY . pegaseengine
RUN apt update && apt install -y make
RUN apt-get install -y freeglut3 freeglut3-dev libglu1-mesa
RUN apt install -y libopengl0
RUN make pegaseengine
CMD /pegaseengine/bin/pegaseengine
