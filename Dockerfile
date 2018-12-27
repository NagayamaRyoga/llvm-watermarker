FROM ubuntu:18.04

COPY . /nykk
WORKDIR /nykk

RUN apt-get update && apt-get install -y --no-install-recommends \
		build-essential \
		ca-certificates \
		curl \
		xz-utils \
	&& rm -rf /var/lib/apt/lists/* \
	&& curl -SL http://releases.llvm.org/7.0.0/clang+llvm-7.0.0-x86_64-linux-gnu-ubuntu-16.04.tar.xz | tar -xJC / \
	&& mv /clang+llvm-7.0.0-x86_64-linux-gnu-ubuntu-16.04 /clang_7.0.0 \
	&& curl https://sh.rustup.rs -sSf | sh -s -- -y --default-toolchain stable

ENV PATH=/clang_7.0.0/bin:$PATH
ENV PATH=/root/.cargo/bin:$PATH
ENV LD_LIBRARY_PATH=/clang_7.0.0/lib:$LD_LIBRARY_PATH

CMD [ "/bin/bash" ]
