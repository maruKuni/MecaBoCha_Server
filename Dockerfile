FROM alpine:latest

RUN apk add --update --no-cache build-base

ENV MECAB_VERSION 0.996
ENV IPADIC_VERSION 2.7.0-20070801
ENV mecab_url https://drive.google.com/uc?export=download&id=0B4y35FiV1wh7cENtOXlicTFaRUE
ENV ipadic_url https://drive.google.com/uc?export=download&id=0B4y35FiV1wh7MWVlSDBCSXZMTXM
ENV build_deps 'curl git bash file sudo openssh'
ENV PATH ${PATH}:root/usr/bin
RUN apk upgrade
RUN apk --update --no-cache add ${build_deps}
RUN curl -SL -o mecab-${MECAB_VERSION}.tar.gz ${mecab_url} \
    && tar zxf mecab-${MECAB_VERSION}.tar.gz
RUN apk add openjdk11
RUN cd mecab-${MECAB_VERSION} \
    && ./configure --enable-utf8-only --with-charset=utf8 --prefix=${HOME}/usr\
    && make \
    && make install 
RUN curl -SL -o mecab-ipadic-${IPADIC_VERSION}.tar.gz ${ipadic_url} \
    && tar zxf mecab-ipadic-${IPADIC_VERSION}.tar.gz \
    && cd mecab-ipadic-${IPADIC_VERSION} \
    && ./configure --with-charset=utf8 --prefix=${HOME}/usr --with-mecab-config=${HOME}/usr/bin/mecab-config\
    && make \
    && make install 
RUN rm -rf /var/cache/apk/*