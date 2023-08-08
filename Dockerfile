FROM ghcr.io/userver-framework/ubuntu-userver-build-base:v1
LABEL authors="nfrmtk"

WORKDIR /tmp
COPY . .
ENV PREFIX=${PREFIX:-~/.local}
RUN make install
WORKDIR /
RUN rm -rf /tmp
ENTRYPOINT $PREFIX/bin/ratings_service --config $PREFIX/etc/ratings_service/static_config.yaml
