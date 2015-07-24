LiveProxies
===================
Latest Version: **0.4.5**

LiveProxies is a [high-performance](#performance) asynchronous proxy checker.

## Features
 - Utilizes Python scripts to harvest (or scrape) proxy lists
 - Analyzes proxy output headers and determines proxy anonymity being:
 - - When proxy reveals correct IP behind the proxy - **transparent**
 - - When proxy modifies or adds any kind of headers - **anonymous**
 - - When proxy headers match request headers - **max**
 - Doesn't require any external web servers for its interface and internal proxy checking page
 - ...

## Get it running
If you still haven't installed depencencies:
```
apt-get install libevent-dev python2.7-dev libssl-dev libgeoip-dev libpcre3-dev libconfig-dev
```
### Compilation: 
```
cmake .
make
```
#### Installation:
```
mkdir /etc/liveproxies
mkdir /etc/liveproxies/scripts
mv ./docs/liveproxies.conf /etc/liveproxies.conf
nano /etc/liveproxies.conf # Modify configuration here
mkdir /usr/local/share/GeoIP
wget http://geolite.maxmind.com/download/geoip/database/GeoLiteCountry/GeoIP.dat.gz
gunzip GeoIP.dat.gz
mv GeoIP.dat /usr/local/share/GeoIP/
wget http://geolite.maxmind.com/download/geoip/database/GeoIPv6.dat.gz
gunzip GeoIPv6.dat.gz
mv GeoIPv6.dat /usr/local/share/GeoIP/
```
#### Usage:

Now you probably would want to add scraping scripts to `/etc/liveproxies/scripts`.

See `BlogspotGeneric.py` for example script. **NOTE:** All blogspot websites are different, so script doesn't always match with blog HTML.

After running, you can see your proxy lists at
 - [ip]:[port]/iface - **Valid proxy list**
 - [ip]:[port]/ifaceu - **Currently unchecked proxy list**
 
See [Auth](docs/auth.md) for preparing users for interface pages

## Dependencies
 - libevent >= 2.0.2-alpha
 - [libevhtp] (forked from [original libevhtp] with fixed bug)
 - python >= 2.7
 - [Maxmind's GeoIP]
 - libconfig
 - pcre
 - openssl

## Development
The program is currently in its very early stages, so any help is appreciated. See TODO list.

## Performance <a name="performance"></a>
Currently, program doesn't slow down at all at 7000 simultaneous proxy checks, but has memory problems. 85 MB for 7000 simultaneous proxy checks seems high.

## TODO
 - ~~Adapt SSL for interface and proxies~~
 - Implement full proxy check
 - Provide more information on interface pages, fix formatting
 - Provide more interactivity on interface pages
 - Make HTML template system for interface pages
 - Interface pages needs design **ASAP**! (seriously, they look hideous)
 - Implement checked proxy checking.
 
 - ...
 

[Maxmind's GeoIP]:https://github.com/maxmind/geoip-api-c/
[libevhtp]:https://github.com/TETYYS/libevhtp
[original libevhtp]:https://github.com/ellzey/libevhtp