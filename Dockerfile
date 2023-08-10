FROM ghcr.io/userver-framework/ubuntu-userver-build-base:v1
LABEL authors="nfrmtk"

WORKDIR /tmp
COPY . .
ENV PREFIX=${PREFIX:-~/.local}
RUN make install
WORKDIR /
RUN rm -rf /tmp
ENTRYPOINT /root/.local/bin/ratings_service --config /root/.local/etc/ratings_service/static_config.yaml
# todo no hardcoding