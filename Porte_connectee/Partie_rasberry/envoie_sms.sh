#!/bin/bash
echo $1
echo $2
curl -X POST  https://rest.nexmo.com/sms/json \
-d api_key=84f53878 \
-d api_secret=c2601e8c3f825910 \
-d to=$1 \
-d from="Porte" \
-d text="Votre code est $2"
