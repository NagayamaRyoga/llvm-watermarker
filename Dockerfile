FROM ubuntu:18.04

COPY . /nykk
WORKDIR /nykk

RUN apt update && apt install -y \
		xz-utils \
		build-essential \
		curl \
	&& rm -rf /var/lib/apt/lists/* \
	&& curl -SL http://releases.llvm.org/7.0.0/clang+llvm-7.0.0-x86_64-linux-gnu-ubuntu-16.04.tar.xz | tar -xJC / \
	&& mv /clang+llvm-7.0.0-x86_64-linux-gnu-ubuntu-16.04 /clang_7.0.0 \
	&& echo 'export PATH=/clang_7.0.0/bin:$PATH' >> ~/.bashrc \
	&& echo 'export LD_LIBRARY_PATH=/clang_7.0.0/lib:LD_LIBRARY_PATH' >> ~/.bashrc

CMD [ "/bin/bash" ]
