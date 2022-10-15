FROM debian:bullseye

RUN apt update && apt install -y apache2

RUN a2enmod proxy proxy_http cache file_cache headers expires cache_disk


RUN rm /etc/apache2/sites-enabled/000-default.conf
COPY server.conf /etc/apache2/sites-enabled/
COPY static/ /var/www/html/

EXPOSE 80

CMD  [ "apachectl", "-D", "FOREGROUND"]
# CMD tail -f /dev/null