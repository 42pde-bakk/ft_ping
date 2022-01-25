FROM debian:9
WORKDIR /code

########################################################
# Essential packages for remote debugging and login in
########################################################

# RUN apt-get update && apt-get upgrade -y && apt-get install -y \
#     apt-utils gcc g++ openssh-server cmake build-essential gdb gdbserver rsync vim
#
# RUN mkdir /var/run/sshd
# RUN echo 'root:root' | chpasswd
# RUN sed -i 's/PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config
#
# # SSH login fix. Otherwise user is kicked off after login
# RUN sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd
#
# ENV NOTVISIBLE "in users profile"
# RUN echo "export VISIBLE=now" >> /etc/profile
#
# # 22 for ssh server. 7777 for gdb server.
# EXPOSE 22 7777
#
# RUN useradd -ms /bin/bash debugger
# RUN echo 'debugger:pwd' | chpasswd

########################################################
# Add custom packages and development environment here
########################################################

########################################################


RUN apt-get update && apt-get upgrade -y
RUN apt-get install -y gcc make valgrind tcpdump wget git curl
RUN echo "done installing apt packages"

# Uses "robbyrussell" theme (original Oh My Zsh theme), with no plugins
#RUN sh -c "$(wget -O- https://github.com/deluan/zsh-in-docker/releases/download/v1.1.2/zsh-in-docker.sh)" -- \
#    -t robbyrussell

#RUN wget https://github.com/robbyrussell/oh-my-zsh/raw/master/tools/install.sh -O - | zsh || true
#RUN sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)" "" --unattended
#RUN echo "Done installing oh-my-zsh"

# CMD ["/usr/sbin/sshd", "-D"]
