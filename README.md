# ratings_service

C++ CRUD service that uses [userver framework](https://github.com/userver-framework/userver) with PostgreSQL.

## Features
Basic features are described [here](./openapi.yaml)

## TODO
1. ~email/password validity/strogness check~ DONE
2. ~'sign out' handler~ DONE
3. ~hub.docker.com push on every succesfull merge~ almost done, dockerhub push on every succesfull release branch push
4. nginx support
## Download and Build

```shell
git clone https://github.com/nfrmtk/ratings
cd ratings
git submodule update --init
make tests
make docker-start-service-release
```

## Makefile

Makefile contains typicaly useful targets for development:

* `make build-debug` - debug build of the service with all the assertions and sanitizers enabled
* `make build-release` - release build of the service with LTO
* `make test-debug` - does a `make build-debug` and runs all the tests on the result
* `make test-release` - does a `make build-release` and runs all the tests on the result
* `make service-start-debug` - builds the service in debug mode and starts it
* `make service-start-release` - builds the service in release mode and starts it
* `make` or `make all` - builds and runs all the tests in release and debug modes
* `make format` - autoformat all the C++ and Python sources
* `make clean-` - cleans the object files
* `make dist-clean` - clean all, including the CMake cached configurations
* `make install` - does a `make build-release` and runs install in directory set in environment `PREFIX`
* `make install-debug` - does a `make build-debug` and runs install in directory set in environment `PREFIX`
* `make docker-COMMAND` - run `make COMMAND` in docker environment
* `make docker-build-debug` - debug build of the service with all the assertions and sanitizers enabled in docker environment
* `make docker-test-debug` - does a `make build-debug` and runs all the tests on the result in docker environment
* `make docker-start-service-release` - does a `make install-release` and runs service in docker environment
* `make docker-start-service-debug` - does a `make install-debug` and runs service in docker environment
* `make docker-clean-data` - stop docker containers and clean database data

Edit `Makefile.local` to change the default configuration and build options.


## License

The original template is distributed under the [Apache-2.0 License](https://github.com/userver-framework/userver/blob/develop/LICENSE)
and [CLA](https://github.com/userver-framework/userver/blob/develop/CONTRIBUTING.md). Services based on the template may change
the license and CLA.
