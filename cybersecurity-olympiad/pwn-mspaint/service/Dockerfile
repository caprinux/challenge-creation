FROM ubuntu:22.04

RUN apt-get -y update
RUN apt-get install -y socat && rm -rf /var/lib/apt/lists/*

RUN useradd ctf
WORKDIR /home/ctf

COPY ./chall /home/ctf/chall
COPY ./flag.txt /home/ctf/flag.txt

RUN chown -R root /home/ctf
RUN chmod 641 /home
RUN chmod 555 chall
RUN chmod 444 flag.txt
RUN chmod 770 /tmp

CMD ["socat", "TCP-LISTEN:5000,reuseaddr,fork", "EXEC:/home/ctf/chall,stderr"]

EXPOSE 5000
