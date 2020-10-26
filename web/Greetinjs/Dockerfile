FROM httpd:2.4-alpine

ADD challenge $HTTPD_PREFIX/htdocs
RUN sed -i "s/Options Indexes FollowSymLinks/Options FollowSymLinks/" $HTTPD_PREFIX/conf/httpd.conf