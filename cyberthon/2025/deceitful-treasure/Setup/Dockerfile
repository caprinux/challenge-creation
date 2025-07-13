FROM ubuntu:22.04 AS app

FROM pwn.red/jail

COPY --from=app / /srv
COPY ./chall /srv/app/run

RUN chmod 111 /srv/app/run
