FROM ubuntu:22.04 AS app

COPY chall /app/run
COPY flag.txt /app/flag.txt

# You don't need to care about this
FROM pwn.red/jail
COPY --from=app / /srv
