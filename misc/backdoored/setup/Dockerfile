FROM ubuntu
ARG USERNAME=user
ENV USERNAME ${USERNAME}
RUN useradd -m $USERNAME
RUN apt-get update && apt-get install -y \
    xinetd \
	vim \
    && rm -rf /var/lib/apt/lists/*

ADD ./backdoor /tmp/backdoor
ADD xinetd.conf /etc/xinetd.d/chal
RUN chmod +x /tmp/backdoor
RUN mkdir /tmp/secret
ADD ./evil.sh /tmp/secret/.evil.sh
RUN mkdir -p /tmp/.vim/undo
ADD ./evil.sh.un /tmp/.vim/undo/..evil.sh.un~
ADD ./vimrc /home/user/.vimrc
ADD ./history /home/user/.bash_history
ADD ./bashrc /home/user/.bashrc
RUN chown user:user /home/user
RUN chmod 777 /home/user
ENV HOME /home/user
EXPOSE $PORT

CMD [ \
        "xinetd", \
        "-dontfork", \
        "-f", "/etc/xinetd.d/chal" \
    ]
