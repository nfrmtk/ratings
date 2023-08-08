FROM ghcr.io/userver-framework/ubuntu-userver-build-base:v1
LABEL authors="nfrmtk"

WORKDIR /tmp
COPY . .
RUN make dist-clean # just in case
ENV PREFIX=${PREFIX:-~/.local}
RUN make install
RUN find / -name ratings_service 2> /dev/null
WORKDIR /
RUN rm -rf /tmp
ENTRYPOINT /usr/local/bin/ratings_service --config /usr/local/etc/ratings_service/static_config.yaml
