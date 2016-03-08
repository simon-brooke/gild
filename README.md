# GILD

## The Generic Internet Listener Daemon

This is ancient history, and never was meant for doing anything practical; it's essentially
a joke. Its practical use - when it had any practical use - was to experiment with new 
protocols.

The fact that protocols can be implemented in more or less any language you like is an added
jape: see HTTP in under 100 lines of shell script.

Enjoy!

## Building

    pushd src; make clean; make; popd

## Running

It depends what you want to do with it. Review the contents of `gild.conf`; in practice only the HTTP handler is going to work out of the box, and, out of the box, it expects to find the website to serve in `/usr/local/etc/gild/httpd/htdocs`. To change where it looks for its website, edit the HTTP handler.

The HTTP handler will serve HTML files, plain text files and common image formats, but nothing more sophisticated. It currently won't serve CSS correctly (although your browser will probably cope) because it can't tell the difference between CSS and plain text.

Having said all that, to run it,

    bin/gild -f gild.conf

will start `gild` listening on its default port, 8421. If you'd prefer it to listen on, for example, port 6666, you would use

    bin/gild -f gild.conf -p 6666

_Be aware that gild is a true daemon_. It will immediately detach from the console, and you will get your prompt back as if nothing had happened. But if you look in the syslog, you'll see something like this:

    Mar  8 19:05:25 fletcher gild: Loading configuration from gild.conf
    Mar  8 19:05:25 fletcher gild: registering handler [handlers/http] for protocol http
    Mar  8 19:05:25 fletcher gild: registering handler [handlers/crp] for protocol crp
    Mar  8 19:05:25 fletcher gild: registering handler [handlers/mdlscp] for protocol mdlscp
    Mar  8 19:05:25 fletcher gild: started; awaiting requests on port 6666
    Mar  8 19:06:14 fletcher gild: using handler 'handlers/http' [28001] to handle http request from 127.0.0.1

Back in the day, `gild` really did run a public website for several years. However, back in the day, the internet was a much less hostile and dangerous place. _It is not at all security hardened_. If you're going to run it on a public network, do it on a Raspberry Pi or something of that nature which has nothing you value on it, and keep a close eye on what happens.

## Using

If you have a bright idea for a completely new application layer protocol, `gild` is a good tool for prototyping, because it's absurdly easy to get things working. Apart from that, I strongly advise you to treat it as an amusing curiousity, only.

## License

GPL version 2. You never were going to use this for anything commercial, seriously.
