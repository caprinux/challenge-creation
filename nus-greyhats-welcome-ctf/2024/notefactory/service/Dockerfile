FROM python:3.10-slim-buster as base

RUN apt-get update \
        && apt-get install -y socat \
        && rm -rf /var/lib/apt/lists/*

COPY flag.txt /flag.txt
RUN chmod 444 /flag.txt
COPY challenge.py /challenge.py
RUN chmod 555 /challenge.py

ENV PYTHONUNBUFFERED=1

CMD socat TCP-LISTEN:5000,reuseaddr,fork EXEC:/challenge.py,stderr
