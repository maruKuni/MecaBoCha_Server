FROM alpine:latest

RUN apk add --update --no-cache build-base

ENV MECAB_VERSION 0.996
ENV IPADIC_VERSION 2.7.0-20070801
ENV mecab_url https://drive.google.com/uc?export=download&id=0B4y35FiV1wh7cENtOXlicTFaRUE
ENV ipadic_url https://drive.google.com/uc?export=download&id=0B4y35FiV1wh7MWVlSDBCSXZMTXM
ENV crf_url https://drive.google.com/uc?export=download&id=0B4y35FiV1wh7QVR6VXJ5dWExSTQ
ENV CRF_VERSION 0.58
ENV FILE_ID 0B4y35FiV1wh7SDd1Q1dUQkZQaUU
ENV FILE_NAME cabocha-0.69.tar.bz2
ENV build_deps 'curl git bash file sudo openssh openssl'
ENV PATH ${PATH}:/root/usr/bin
RUN apk upgrade
RUN apk --update --no-cache add ${build_deps}
RUN curl -SL -o mecab-${MECAB_VERSION}.tar.gz ${mecab_url} \
    && tar zxf mecab-${MECAB_VERSION}.tar.gz
RUN cd mecab-${MECAB_VERSION} \
    && ./configure --enable-utf8-only --with-charset=utf8 --prefix=${HOME}/usr\
    && make \
    && make install \ 
    && mv /root/usr/include/mecab.h /usr/include/mecab.h
RUN curl -SL -o mecab-ipadic-${IPADIC_VERSION}.tar.gz ${ipadic_url} \
    && tar zxf mecab-ipadic-${IPADIC_VERSION}.tar.gz \
    && cd mecab-ipadic-${IPADIC_VERSION} \
    && ./configure --with-charset=utf8 --prefix=${HOME}/usr --with-mecab-config=${HOME}/usr/bin/mecab-config\
    && make \
    && make install 
RUN curl -SL -o CRF++-${CRF_VERSION}.tar.gz ${crf_url} \  
    && tar zxf CRF++-${CRF_VERSION}.tar.gz \
    && cd CRF++-${CRF_VERSION} \   
    && ./configure \ 
    && make \ 
    && sudo make install  
RUN  curl -sc /tmp/cookie "https://drive.google.com/uc?export=download&id=${FILE_ID}" > /dev/null \ 
    && CODE="$(awk '/_warning_/ {print $NF}' /tmp/cookie)" \   
    && curl -Lb /tmp/cookie "https://drive.google.com/uc?export=download&confirm=${CODE}&id=${FILE_ID}" -o ${FILE_NAME} \ 
    && bzip2 -dc cabocha-0.69.tar.bz2 | tar xvf - 
RUN cd cabocha-0.69 \ 
    && ./configure --with-mecab-config=`which mecab-config` --with-charset=UTF8 \ 
    && make \ 
    && make check \   
    && sudo make install  
RUN rm -rf /var/cache/apk/*