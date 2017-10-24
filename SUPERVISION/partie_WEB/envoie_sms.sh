#!/bin/bash

curl -X POST  https://rest.nexmo.com/sms/json \
-d api_key=########## \
-d api_secret=################### \
-d to=33662098810 \
-d from="Supervision" \
-d text="ip $1 $2 ne repond plus. Lanestel"
