### How to run this service

Linux/Windows/Macos(x86): 


```shell
curl -o docker-compose.yaml \
https://raw.githubusercontent.com/nfrmtk/ratings/release/docker/prebuilt.yaml \
&& docker-compose up 
```

Macos(arm):
No arm64/v8 runner on github actions yet, so have to build image locally  
```shell
git clone https://github.com/nfrmtk/ratings
cd ratings && git submodule update --init && cd docker
docker build -t nfrmtk/ratings service
docker build -t nfrmtk/ratings-postgres postgres-with-schema
docker-compose up 
```
