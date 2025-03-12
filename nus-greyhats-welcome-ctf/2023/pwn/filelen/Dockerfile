FROM pwn.red/jail
COPY --from=ubuntu / /srv

RUN mkdir /srv/app
ADD ./app/chall /srv/app/run
ADD ./app/flag.txt /srv/app/flag.txt

ENV JAIL_TIME 60
