FROM ubuntu:latest

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get -y update --fix-missing && apt-get -y upgrade
RUN apt-get -y install ruby \
                       ruby-dev \
                       build-essential
                       # libxml2-dev \

RUN gem install bundler
RUN groupadd -r markdamnit && useradd -r -g markdamnit markdamnit

ENV APP_ROOT /home/markdamnit/
WORKDIR ${APP_ROOT}

ADD challenge/Gemfile ${APP_ROOT}
ADD challenge/Gemfile.lock ${APP_ROOT}
ADD challenge/templates/ ${APP_ROOT}/templates/
ADD challenge/app.rb ${APP_ROOT}
ADD challenge/config.ru ${APP_ROOT}
ADD challenge/example.md ${APP_ROOT}
ADD challenge/flag.txt ${APP_ROOT}
ADD challenge/puma.conf.rb ${APP_ROOT}
ADD challenge/init.sh /etc/init.sh

RUN bundle config set without 'development test'
RUN bundle install -j4

RUN chown -R root:root ${APP_ROOT}
RUN chown root:root ${APP_ROOT}
RUN chmod 755 templates
RUN chmod 644 templates/index.html.erb
RUN chmod 644 Gemfile
RUN chmod 644 Gemfile.lock
RUN chmod 755 app.rb
RUN chmod 755 config.ru
RUN chmod 644 flag.txt
RUN mv flag.txt flag-$(sha256sum flag.txt | awk '{print $1}').txt

RUN chmod 555 /etc/init.sh
RUN chmod 1733 /tmp /var/tmp /dev/shm
USER markdamnit

RUN ls -lh ${app}
RUN whoami
