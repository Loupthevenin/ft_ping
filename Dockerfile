FROM debian:bullseye

RUN apt-get update && \
	apt-get install -y gcc make iputils-ping net-tools && \
	apt-get clean

WORKDIR /ft_ping
