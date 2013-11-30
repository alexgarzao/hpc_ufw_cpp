g++ -W -Wall -Wextra -Isrc/ -Itests/ tests/server1.cc src/concurrency_model.cc src/standard_socket_interface.cc src/ufw.cc src/multiply_numbers.cc -lev -o bin/server1 && \
g++ -W -Wall -Wextra -Isrc/ -Itests/ tests/client_request1.cc -o bin/cr1
