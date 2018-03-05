#!/bin/sh
if [ -f /etc/init/ofdpa.conf ]; then
    status ofdpa | grep -i running >>/dev/null 2>&1 && stop ofdpa
fi
if [ -f /etc/init/ofagentapp.conf ]; then
    status ofagentapp | grep -i running >>/dev/null 2>&1 && stop ofagentapp
fi
exit 0
