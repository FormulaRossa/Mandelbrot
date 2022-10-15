FROM debian:bullseye as build


RUN apt update && apt install -y build-essential cmake

COPY .. /build/
WORKDIR /build/

RUN cmake .
RUN make


FROM debian:bullseye

RUN apt update && apt install -y apache2

RUN ln -s /etc/apache2/mods-available/cgi.load /etc/apache2/mods-enabled/

COPY --from=build /build/Mandelbrot /usr/lib/cgi-bin/mandelbrot.cgi
RUN rm /etc/apache2/sites-enabled/000-default.conf
COPY mandelbrot.conf /etc/apache2/sites-enabled/

EXPOSE 80

CMD  [ "apachectl", "-D", "FOREGROUND"]
# CMD tail -f /dev/null