FROM pwn.red/jail
COPY --from=ubuntu:22.04 / /srv

COPY i_am_worthy /srv/app/i_am_worthy
COPY challenge /srv/app/run

RUN chmod +x /srv/app/run

ENV JAIL_MEM 15M
