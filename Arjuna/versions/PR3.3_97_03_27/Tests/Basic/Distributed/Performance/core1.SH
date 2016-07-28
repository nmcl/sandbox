echo "** Core RPC system Performance"

OperInitTermDriver -num 500

echo "** Parameter Transfer Performance"

OperParamSizeDriver -size 0 -num 1000
OperParamSizeDriver -size 256 -num 1000
OperParamSizeDriver -size 512 -num 1000
OperParamSizeDriver -size 1024 -num 1000
OperParamSizeDriver -size 2048 -num 1000
OperParamSizeDriver -size 4096 -num 1000

