FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    git cmake g++ make

WORKDIR /app

RUN git clone https://github.com/MrKokao2/mp-sequence-lab.git .

WORKDIR /app/lab2

RUN cmake -S . -B build
RUN cmake --build build

CMD ["./build/search_tests"]