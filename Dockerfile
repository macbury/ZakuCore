FROM node:16.13.1
RUN apt-get update && apt-get install -y python3 python3-pip

WORKDIR /release
COPY package.json yarn.lock requirements.txt /release/
RUN pip3 install -r requirements.txt
RUN yarn
COPY . /release
RUN chmod +x /release/scripts/release_all.sh

ENTRYPOINT [ "/release/scripts/release_all.sh" ]
