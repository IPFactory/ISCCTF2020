FROM node:14.3.0-alpine3.10

LABEL "com.github.actions.name"="Markdownlint"
LABEL "com.github.actions.description"="Lint all Markdown style"
LABEL "com.github.actions.icon"="search"
LABEL "com.github.actions.color"="green"

LABEL "repository"="https://github.com/ipfactory/ISCCTF2020"
LABEL "maintainer"="ipfactory"

RUN npm install -g markdownlint-cli

ADD entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]
