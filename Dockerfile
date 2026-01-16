# корневой image образ для сборки - чистый ubuntu
# FROM dockerscripts/linuxmint
FROM ubuntu
# обновляем apt пакеты и устанавливаем по очереди нужные зависимости
RUN apt-get update && apt-get install -y \
    gcc \
    # golang-go\
    golang-go \
    swig\
    git\
    wget\
    libgl1-mesa-dev\
    xorg-dev\
    make

# Устанавливаем Go (if not already installed)
# RUN wget https://dl.google.com/go/go1.23.linux-amd64.tar.gz && \
#     tar -C /usr/local -xzf go1.23.linux-amd64.tar.gz && \
#     rm go1.23.linux-amd64.tar.gz
#    go version
#    check \
#    llvm \
#    lcov \
#    libpcre3 \
#    libpcre3-dev \
    # libsdl2-dev \
    # libsdl2-image-dev \
    # libsdl2-mixer-dev \
    # libsdl2-ttf-dev
    
# RUN mkdir -p /run/user/1000
# ENV XDG_RUNTIME_DIR=/run/user/1000 
ENV XDG_RUNTIME_DIR=/tmp/.X11-unix \ 
    PATH=$PATH:/usr/local/go/bin

# RUN go version
    # DISPLAY=unix$DISPLAY
COPY . .

# Устанавливаем 
WORKDIR /.


# CMD /bin/bash
