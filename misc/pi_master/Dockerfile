FROM python:3 AS src

FROM redpwn/jail:latest

ENV JAIL_TIME 60
ENV JAIL_CPU 0
ENV JAIL_MEM 0

COPY --from=src / /srv/
COPY ./chall.py /srv/app/run
COPY ./Pi125MDP.txt /srv/app/
COPY ./flag.txt /srv/app/

RUN chmod +x /srv/app/run
