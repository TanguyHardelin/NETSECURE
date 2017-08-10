#!/bin/bash

curl -X POST  https://rest.nexmo.com/sms/json \
-d api_key=84f53878 \
-d api_secret=c2601e8c3f825910 \
-d to=33662098810 \
-d from="Supervision" \
-d text="ip $1 $2 ne repond plus. Lanestel"
