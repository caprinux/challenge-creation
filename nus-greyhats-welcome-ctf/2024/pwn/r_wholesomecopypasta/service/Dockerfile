# source filesystem
FROM ubuntu:22.04 AS app
# apt install any dependencies if necessary
RUN apt-get update && rm -rf /var/lib/apt/lists/* 

# this is the actual container that runs
FROM pwn.red/jail
# copy ubuntu file system into our redpwn jail
COPY --from=app / /srv
# copy local file into docker
COPY ./challenge /srv/app/run
# copy flag into docker if needed
COPY ./*.txt /srv/app
