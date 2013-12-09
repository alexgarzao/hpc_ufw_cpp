echo "Building server1" && \
g++ -W -Wall -Wextra -Isrc/ -I/opt/local/include/ tests/server1.cc src/concurrency_model.cc src/worker_thread.cc src/standard_socket_interface.cc src/ufw.cc tests/multiply_numbers.cc -lev -L/opt/local/lib/ -lboost_thread-mt -lboost_system-mt -o bin/server1 && \
echo "Building client_request_1" && \
g++ -W -Wall -Wextra -Isrc/ -Itests/ tests/client_request1.cc -o bin/cr1
echo "Building client_multiple_requests1" && \
g++ -W -Wall -Wextra -Isrc/ -Itests/ tests/client_multiple_requests1.cc -o bin/cmr1
