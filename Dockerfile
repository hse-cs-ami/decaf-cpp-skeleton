FROM ubuntu:24.04

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    default-jre-headless \
    libantlr4-runtime-dev \
    uuid-dev \
    ca-certificates \
    wget \
 && rm -rf /var/lib/apt/lists/*

# antlr4-complete.jar должен быть той же версии, что и libantlr4-runtime-dev (4.10).
RUN wget -q -O /usr/local/lib/antlr4-complete.jar \
    https://www.antlr.org/download/antlr-4.10.1-complete.jar

ENV ANTLR_JAR=/usr/local/lib/antlr4-complete.jar

WORKDIR /src
