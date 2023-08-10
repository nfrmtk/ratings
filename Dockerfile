FROM ghcr.io/userver-framework/ubuntu-userver-build-base:v1
LABEL authors="nfrmtk"

WORKDIR /tmp
COPY . .
ENV PREFIX=${PREFIX:-~/.local}
RUN make install
WORKDIR /
RUN rm -rf /tmp
RUN sed -i 's/config_vars.yaml/config_vars.docker.yaml/g' /root/.local/etc/ratings_service/static_config.yaml
RUN psql 'postgresql://user:password@service-postgres:5432/ratings_service_db-1' -f ./postgresql/data/initial_data.sql
ENTRYPOINT /root/.local/bin/ratings_service --config /root/.local/etc/ratings_service/static_config.yaml
# todo no hardcoding