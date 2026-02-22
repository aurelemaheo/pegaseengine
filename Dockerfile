FROM ubuntu:22.04
COPY build pegaseengine
#RUN apt update && apt install -y make
#RUN apt-get install -y freeglut3 freeglut3-dev libglu1-mesa
#RUN apt install -y libopengl0
#RUN apt-get install -qqy x11-apps
#RUN make pegaseengine
CMD /pegaseengine/bin/case2
EXPOSE 3000
